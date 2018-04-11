#include <string.h>
#include <stddef.h>
#include <errno.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <libcommon.h>

#include <utils.h>
#include <thread.h>

#define DOMAIN_LISTEN_MAX   1

typedef struct domain_cli_info_s
{
    int     timerfd;
    int     domainfd;
    char    *cli;
    char    *domain;
} domain_cli_info_t;

static domain_cli_info_t domain_cli_info;


static
void domain_client_loop_connect(int timerfd, void *arg, uint32_t events);

static
int domain_fd_create(const char *domain)
{
    int                     fd, size;
    int                     ret;
    struct sockaddr_un      un;

    un.sun_family = AF_UNIX;
    un.sun_path[0] = 0;
    strcpy(un.sun_path + 1, domain);

    fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0)
    {
        DEBUG_MSG("socket failed");
    }

    size = offsetof(struct sockaddr_un, sun_path) + strlen(domain) + 1;
    ret = bind(fd, (struct sockaddr *)&un, size);
    if (RET_FALIURE(ret))
    {
        DEBUG_MSG("bind failed %s", strerror(ret));
        return ret;
    }

    return fd;
}


static
void domain_fd_read(int fd, void *arg, uint32_t events)
{
    int     n;
    uchar   buf[1024];


    UNUSED(arg);
    UNUSED(events);

    n = read(fd, buf, sizeof(buf));
    if (RET_FALIURE(n))
    {
        DEBUG_MSG("read error");
    }

    buf[n] = 0;

    DEBUG_MSG("recv:%s", buf);
}


static
void domain_fd_accept(int fd, void *arg, uint32_t events)
{
    int                 clifd;
    uint                len;
    struct sockaddr_un  un;


    UNUSED(arg);
    UNUSED(events);

    len = sizeof(un);
    clifd = accept(fd, (struct sockaddr *)&un, &len);
    if (RET_FALIURE(clifd))
    {
        DEBUG_MSG("accept error %s", strerror(errno));
        return ;
    }

    thread_fd_read_add(clifd, domain_fd_read, NULL);

}


int domain_server_init(const char * domain)
{
    int     fd;
    int     ret;


    thread_create();

    fd = domain_fd_create(domain);
    if (RET_FALIURE(fd))
    {
        DEBUG_MSG("create err %s", strerror(fd));
        return fd;
    }

    ret = listen(fd, DOMAIN_LISTEN_MAX);
    if (RET_FALIURE(ret))
    {
        DEBUG_MSG("listen %s", strerror(ret));
        return ret;
    }

    thread_fd_read_add(fd, domain_fd_accept, NULL);

    thread_run();

    return RET_OK;
}


static
int domain_client_connect(int fd, const char *domain)
{
    int     ret;
    uint    len;
    struct sockaddr_un un;


    BZEOR(un);
    un.sun_family = AF_UNIX;
    strcpy(un.sun_path + 1, domain);

    len = offsetof(struct sockaddr_un, sun_path) + strlen(domain) + 1;

    ret = connect(fd, (struct sockaddr *)&un, len);
    if (RET_FALIURE(ret))
    {
        DEBUG_MSG("connect error %s", strerror(errno));
        return RET_SYS;
    }

    return RET_OK;
}


static
void domain_cli_read(int fd, void *arg, uint32_t events)
{
    char    buf[1024];
    int     nbytes;
    int     timerfd;


    UNUSED(arg);

    if (events == FD_EVENT_ERROR)
    {
        timerfd = thread_timer_fd_create(1);

        thread_fd_timer_add(timerfd, domain_client_loop_connect, NULL);

        return ;
    }

    nbytes = read(fd, buf, sizeof(buf));
    (void)nbytes;
}


static
void domain_client_loop_connect(int timerfd, void *arg, uint32_t events)
{
    int     ret;


    UNUSED(timerfd);
    UNUSED(arg);
    UNUSED(events);

    if (RET_OK == domain_client_connect(domain_cli_info.domainfd, domain_cli_info.domain))
    {
        ret = write(domain_cli_info.domainfd, "hello", 5);
        (void)ret;

        thread_fd_read_add(domain_cli_info.domainfd, domain_cli_read, NULL);

        thread_fd_del(timerfd);

        close(timerfd);
    }
}


int domain_client_init(const char *domain, const char *cli)
{
    int     fd, timerfd, ret;


    BZEOR(domain_cli_info);

    fd = domain_fd_create(cli);
    if (RET_FALIURE(fd))
    {
        DEBUG_MSG("creat error %s", strerror(fd));
        return fd;
    }

    domain_cli_info.domainfd = fd;
    domain_cli_info.domain = strdup(domain);
    domain_cli_info.cli = strdup(cli);

    thread_create();

    ret = domain_client_connect(fd, domain);
    if (RET_SUCCESS(ret))
    {
        ret = write(fd, "hello", 5);
        (void)ret;

        thread_fd_read_add(fd, domain_cli_read, NULL);
    }
    else
    {
        timerfd = thread_timer_fd_create(1);

        thread_fd_timer_add(timerfd, domain_client_loop_connect, NULL);
    }

    return thread_run();
}
