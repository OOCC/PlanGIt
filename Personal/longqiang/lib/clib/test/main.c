
#include <libcommon.h>

#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

#include <utils.h>
#include <bitmap.h>
#include <domain.h>
#include <thread.h>

typedef int (*test_func_t)(void);

typedef struct
{
    test_func_t test_fn;
    const char *msg;
} func_test_t;


int bitmap_test()
{
    bitmap_t *new_bitmap;

    new_bitmap = bitmap_alloc(1234);
    assert(new_bitmap);

    bitmap_bit_set(new_bitmap, 0);
    bitmap_bit_set(new_bitmap, 1);
    bitmap_bit_set(new_bitmap, 2);
    bitmap_bit_set(new_bitmap, 3);

    assert(4 == bitmap_bit_alloc(new_bitmap));

    bitmap_bit_unset(new_bitmap, 0);
    bitmap_bit_unset(new_bitmap, 1);
    bitmap_bit_unset(new_bitmap, 2);
    bitmap_bit_unset(new_bitmap, 3);
    bitmap_bit_unset(new_bitmap, 4);

    assert(0 == bitmap_bit_alloc(new_bitmap));

    bitmap_destory(new_bitmap);

    return 0;
}


int domain_test()
{
    pid_t   pid;
    int     fd;


    pid = fork();
    //child process
    if (0 == pid)
    {
        fd = domain_client_init("test", "cli1");

        UNUSED(fd);

        fflush(NULL);

        exit(0);
    }

    sleep(2);

    domain_server_init("test");

    return RET_OK;
}


static
void thread_cb_test(void *arg)
{
    size_t     t = (size_t)arg;


    DDEBUG_MSG("cb %zu", t);
}


static
void thread_fd_cb_test(int fd, void *arg, uint32_t events)
{
    UNUSED(arg);
    UNUSED(fd);


    DDEBUG_MSG("timer %u", events);
}


static
void* thread_new_test(void *a)
{
    int         timerfd;
    size_t      arg = 0;

    UNUSED(a);

    while (10 != arg)
    {
        thread_event_add(thread_cb_test, (void *)++arg);
    }

    timerfd = thread_timer_fd_create(3);

    thread_fd_timer_add(timerfd, thread_fd_cb_test, NULL);


    return NULL;
}


int thread_test()
{
    pthread_t   pid;


    thread_create();

    pthread_create(&pid, NULL, thread_new_test, NULL);

    thread_run();

    pthread_join(pid, NULL);

    return RET_OK;
}


func_test_t test_func[] =
{
    {bitmap_test, "bitmap module"},
    {domain_test, "domain module"},
//    {thread_test, "thread module"},
};


int main()
{
    int     i;
    int     ret;
    int     num = ARRAR_COUNT(test_func);


    for (i = 0; i < num; i++)
    {
        ret = test_func[i].test_fn();
        fprintf(stderr, "%s test %s.\n", test_func[i].msg, 0 == ret ? "pass" : "failed");
        assert(0 == ret);
    }

    fprintf(stderr,"test OK!!!\n");
    return 0;
}
