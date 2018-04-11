#include <libcommon.h>
#include <utils.h>

#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <assert.h>
#include <pthread.h>
#include <sys/epoll.h>
#include <sys/timerfd.h>

#include <thread.h>
#include <list.h>

#define FD_SIZE         1000
#define EVENT_MAX       20

#define PIPE_READ       0
#define PIPE_WRITE      1

#define EVENTS_ERR      (EPOLLERR|EPOLLHUP)

typedef struct thread_info_s thread_info_t;

typedef int (*thread_handle_t)(thread_info_t *);

struct thread_info_s
{
    int                 fd;
    void                *arg;
    uint32_t            events;
    thread_handle_t     read_handle;
    thread_handle_t     write_handle;
};

typedef struct event_list_s
{
    struct list_head    node;
    event_cb            ev_cb;
    void                *cb_arg;
} event_list_t;


typedef struct event_head_s
{
    struct list_head    node;
    pthread_mutex_t     lock;
} event_head_t;


typedef struct thread_rw_s
{
    fd_cb   cb;
    int     timeout;
    void    *cb_arg;
} thread_fd_t;

typedef struct thread_master_s
{
    int             epollfd;
    int             pipefd[2];
    int             timeout;
    event_head_t    events_list_head;
} thread_master_t;

static thread_master_t thread_master;


static inline
void* thread_malloc(const size_t size)
{
    return malloc(size);
}


static inline
void thread_free(void *p)
{
    return free(p);
}


static
thread_info_t* thread_info_get(int fd, thread_handle_t read_handle,
                               thread_handle_t write_handle, void *arg)
{
    thread_info_t *thread;


    thread = thread_malloc(sizeof(thread_info_t));
    if (!thread)
    {
        return NULL;
    }

    thread->fd = fd;
    thread->read_handle = read_handle;
    thread->write_handle = write_handle;
    thread->arg = arg;

    return thread;
}

static
int set_non_blocking(int fd)
{
    int opts;


    opts = fcntl(fd, F_GETFL);
    if (RET_FALIURE(opts))
    {
        DEBUG_MSG("fcntl get %s", strerror(errno));
        return RET_SYS;
    }

    opts = opts|O_NONBLOCK;

    if (fcntl(fd, F_SETFL, opts) < 0)
    {
        DEBUG_MSG("fcntl set %s", strerror(errno));
        return RET_SYS;
    }

    return RET_OK;
}


static
int epoll_fd_create()
{
    thread_master_t *master = &thread_master;


    master->epollfd = epoll_create(FD_SIZE);
    if (RET_FALIURE(master->epollfd))
    {
        DEBUG_MSG("epoll craate err: %s", strerror(errno));
        return RET_SYS;
    }

    return RET_OK;
}


static
int pipe_create()
{
    int     ret;


    ret = pipe(thread_master.pipefd);
    if (RET_FALIURE(ret))
    {
        DEBUG_MSG("pipe create err: %s", strerror(errno));
        return RET_SYS;
    }

    return RET_OK;
}


static
int thread_fd_event_add(int fd, thread_handle_t read_handle, thread_handle_t write_handle,
                        void *arg, uint32_t events)
{
    int                 ret;
    thread_info_t       *thread;
    thread_master_t     *master = &thread_master;
    struct epoll_event  ev;


    set_non_blocking(fd);

    thread = thread_info_get(fd, read_handle, write_handle, arg);
    if (NULL == thread)
    {
        return RET_NULL_PTR;
    }

    ev.data.ptr = thread;
    ev.events = events|EVENTS_ERR;

    ret = epoll_ctl(master->epollfd, EPOLL_CTL_ADD, fd, &ev);
    if (RET_FALIURE(ret))
    {
        DEBUG_MSG("epoll ctl err %s", strerror(errno));
        return RET_SYS;
    }

    return RET_OK;
}


static
int pipe_fd_read(thread_info_t *thread)
{
    int                 fd, n;
    char                buf;
    struct list_head    *node;
    event_list_t        *event;


    fd = thread->fd;
    node = &thread_master.events_list_head.node;

    pthread_mutex_lock(&thread_master.events_list_head.lock);

    while ((n = read(fd, &buf, sizeof(buf))) > 0)
    {
        event = list_next(node, event_list_t, node);
        if (!event)
        {
            DEBUG_MSG("null event!");
        }
        else
        {
            assert(event->ev_cb);
            event->ev_cb(event->cb_arg);
            list_del(&event->node);
            thread_free(event);
        }
    }

    pthread_mutex_unlock(&thread_master.events_list_head.lock);

    return RET_OK;
}


static
void event_list_init()
{
    list_init(&thread_master.events_list_head.node);

    pthread_mutex_init(&thread_master.events_list_head.lock, NULL);
}


static
void timer_list_init()
{
    thread_master.timeout = -1;
}

int thread_create()
{
    int             ret;


    ret = epoll_fd_create();
    IF_FALIURE_RETURN(ret);

    ret = pipe_create();
    IF_FALIURE_RETURN(ret);

    event_list_init();

    timer_list_init();

    thread_fd_event_add(thread_master.pipefd[PIPE_READ], pipe_fd_read, NULL, NULL, EPOLLIN);

    return ret;
}


static
int event_handle(thread_info_t *thread, uint32_t events)
{
    assert(thread);

    if (EPOLLIN & events)
    {
        assert(thread->read_handle);
        thread->events = FD_EVENT_READ;
        thread->read_handle(thread);
    }

    if (EPOLLOUT & events)
    {
        assert(thread->write_handle);
        thread->events = FD_EVENT_WIRTE;
        thread->write_handle(thread);
        DEBUG_MSG("write debug");
    }

    if (EVENTS_ERR & events)
    {
        //TODO
        thread->events = FD_EVENT_ERROR;

        if (thread->read_handle)
        {
            thread->read_handle(thread);
        }

        if (thread->write_handle)
        {
            thread->write_handle(thread);
        }

        epoll_ctl(thread_master.epollfd, EPOLL_CTL_DEL, thread->fd, NULL);

        DEBUG_MSG("err recv");
    }

    return RET_OK;
}


int thread_run()
{
    int                 nfds, i;
    thread_info_t      *thread;
    thread_master_t     *master = &thread_master;
    struct epoll_event  ev[EVENT_MAX];


    while (1)
    {
        nfds = epoll_wait(master->epollfd, ev, EVENT_MAX, master->timeout);
        IF_FALIURE_RETURN(nfds);

        //timeout
        if (0 == nfds)
        {
            ;
        }
        else
        {
            for (i = 0; i < nfds; ++i)
            {
                thread = ev[i].data.ptr;
                event_handle(thread, ev[i].events);
            }
        }
    }

    return RET_OK;
}


void thread_destory()
{

}


void thread_pipe_test(const char *buf, size_t len)
{
    int     ret;


    ret = write(thread_master.pipefd[PIPE_WRITE], buf, len);
    if (RET_FALIURE(ret))
    {
        DEBUG_MSG("write %s", strerror(errno));
    }
}


static
event_list_t* event_get(event_cb cb, void *arg)
{
    event_list_t    *event;


    event = thread_malloc(sizeof(*event));
    if (!event)
    {
        DEBUG_MSG("thread malloc failed");
        return NULL;
    }

    event->ev_cb = cb;
    event->cb_arg = arg;

    return event;
}


static inline
void thread_awaken()
{
    int             ret;
    char            buf = 0;


    ret = write(thread_master.pipefd[PIPE_WRITE], &buf, sizeof(buf));
    if (RET_FALIURE(ret))
    {
        DEBUG_MSG("write err %s", strerror(errno));
    }
}


int thread_event_add(event_cb cb, void *arg)
{
    event_list_t    *event;


    event = event_get(cb, arg);

    pthread_mutex_lock(&thread_master.events_list_head.lock);

    list_add_tail(&event->node, &thread_master.events_list_head.node);

    pthread_mutex_unlock(&thread_master.events_list_head.lock);

    thread_awaken();

    return RET_OK;
}


static
int thread_read_write_handle(thread_info_t* thread)
{
    thread_fd_t*      rw_cb;


    rw_cb = thread->arg;

    rw_cb->cb(thread->fd, rw_cb->cb_arg, thread->events);

    return RET_OK;
}


int thread_fd_read_add(int fd, fd_cb cb, void *cb_arg)
{
    int                 ret;
    thread_fd_t*        read_cb;


    read_cb = thread_malloc(sizeof(*read_cb));
    IF_NULL_RETURN(read_cb);

    read_cb->cb = cb;
    read_cb->cb_arg = cb_arg;

    ret = thread_fd_event_add(fd, thread_read_write_handle, NULL, read_cb, EPOLLIN);
    if (RET_FALIURE(ret))
    {
        DEBUG_MSG("thread_fd_event_add error %s", rc2msg(ret));
        thread_free(read_cb);
        return ret;
    }

    return RET_OK;
}


int thread_fd_write_add(int fd, fd_cb cb, void *cb_arg)
{
    int                 ret;
    thread_fd_t*        write_cb;


    write_cb = thread_malloc(sizeof(*write_cb));
    IF_NULL_RETURN(write_cb);

    write_cb->cb = cb;
    write_cb->cb_arg = cb_arg;

    ret = thread_fd_event_add(fd, NULL, thread_read_write_handle, write_cb, EPOLLOUT);
    if (RET_FALIURE(ret))
    {
        DEBUG_MSG("thread_fd_event_add error %s", rc2msg(ret));
        thread_free(write_cb);
        return ret;
    }

    return RET_OK;
}


int thread_timer_fd_create(int seconds)
{
    int                 fd;
    struct itimerspec   itime;


    fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
    if (RET_FALIURE(fd))
    {
        DEBUG_MSG("timerfd error %s", strerror(errno));
        return RET_SYS;
    }

    BZEOR(itime);

    itime.it_interval.tv_sec = seconds;
    itime.it_value.tv_sec = seconds;

    timerfd_settime(fd, 0, &itime, NULL);

    return fd;
}


void thread_timer_fd_close(int fd)
{
    close(fd);
}


static
int thread_timer_handle(thread_info_t* thread)
{
    int                 fd;
    thread_fd_t         *timer_cb;
    struct itimerspec   itime;


    BZEOR(itime);

    fd = thread->fd;

    timerfd_gettime(fd, &itime);

    timer_cb = thread->arg;
    timer_cb->cb(fd, timer_cb->cb_arg, thread->events);

    return RET_OK;
}


int thread_fd_timer_add(int fd, fd_cb cb, void *cb_arg)
{
    int                 ret;
    thread_fd_t*        timer_cb;


    timer_cb = thread_malloc(sizeof(*timer_cb));
    IF_NULL_RETURN(timer_cb);

    timer_cb->cb = cb;
    timer_cb->cb_arg = cb_arg;

    ret = thread_fd_event_add(fd, thread_timer_handle, NULL, timer_cb, EPOLLIN|EPOLLET);
    if (RET_FALIURE(ret))
    {
        DEBUG_MSG("thread_fd_event_add error %s", rc2msg(ret));
        thread_free(timer_cb);
        return ret;
    }

    return RET_OK;
}


int thread_fd_del(int fd)
{
    int     ret;


    ret = epoll_ctl(thread_master.epollfd, EPOLL_CTL_DEL, fd, NULL);
    if (RET_FALIURE(ret))
    {
        DEBUG_MSG("fd del %s", strerror(errno));
        return RET_SYS;
    }

    return RET_OK;
}
