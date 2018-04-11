#pragma once

#include <pthread.h>

#define ARRAR_COUNT(array)          (sizeof(array)/sizeof(array[0]))

#define UNUSED(v)                   (void)v

#define IF_FALIURE_RETURN(ret)      do {if (RET_FALIURE(ret)) return ret;} while(0)

#define IF_NULL_RETURN(p)           do {if (!p) return RET_NULL_PTR;} while(0)

#define BZEOR(s)                    bzero(&(s), sizeof(s));

#define DEBUG_MSG(format, ...)      fprintf(stderr, "[%s:%d] " format "\n",\
                                            __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define DDEBUG_MSG(format, ...)     fprintf(stderr, "[%s:%s:%d:%u:%zu] " format "\n",\
                                            clocaltime(), __FUNCTION__, __LINE__,   \
                                            getpid(), pthread_self(), ##__VA_ARGS__)


/**
 * @brief get string local time
 * @return
 * @author Long Qiang
 * @date 2018/2/23 18:25:04
 */
char* clocaltime();

