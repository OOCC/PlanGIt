#pragma once

#define FD_EVENT_READ       0
#define FD_EVENT_WIRTE      1
#define FD_EVENT_ERROR      2

typedef void (*event_cb)(void *);

typedef void (*fd_cb)(int , void *, uint32_t);
/**
 * @brief create a thread
 * @return
 * @author Long Qiang
 * @date 2018/2/19 19:41:51
 */
int thread_create();
/**
 * @brief run thread
 * @note no-block
 * @author Long Qiang
 * @date 2018/2/19 19:47:27
 */
int thread_run();
/**
 * @brief test api
 * @return
 * @author Long Qiang
 * @date 2018/2/19 20:37:24
 */
void thread_pipe_test(const char *buf, size_t len);
/**
 * @brief add a event to thread
 * @return
 * @author Long Qiang
 * @date 2018/2/21 21:57:16
 */
int thread_event_add(event_cb cb, void *arg);
/**
 * @brief   add timer fd
 * @return
 * @author Long Qiang
 * @date 2018/2/23 8:19:54
 */
int thread_fd_timer_add(int fd, fd_cb cb, void *cb_arg);
/**
 * @brief create a timer file descriptor
 * @return
 * @author Long Qiang
 * @date 2018/2/23 8:20:26
 */
int thread_timer_fd_create(int seconds);

/**
 * @brief
 * @author Long Qiang
 * @date 2018/2/23 8:25:09
 */
int thread_fd_read_add(int fd, fd_cb cb, void *cb_arg);

int thread_fd_del(int fd);