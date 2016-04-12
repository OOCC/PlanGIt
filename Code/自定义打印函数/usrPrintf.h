/*************************************************************************
	> File Name: usrPrintf.h
	> Author: marvin
	> Mail: marvinkoala@google.com
	> Created Time: Mon 11 Apr 2016 05:33:34 PM CST
 ************************************************************************/

#ifndef _USRPRINTF_H
#define _USRPRINTF_H

#include <stdio.h>
#include <time.h>
#include "type_def,h"


#define TIME_STR_LEN        64


//func: 重定义标准库函数Printf，以便不定参数传递
//in:
//out:
//ret:
//note:
#define USER_PRINT_BASE(format, args...)     printf(format, ##args)


//func: 自定义打印函数，在需要打印的信息前添加：时间戳、调用该打印函数的源文件名及打印函数所在行号 
//in:
//out:
//ret:
//note:
#define UsrPrintf(format, args...)\
{\
	int8 timeStr[TIME_STR_LEN]={0}; \
	int32 timeStrLen; \
	time_t curTime; \
	\
	time(curTime);\
	strftime(timeStr， timeStrLen, "%Y%m%d%H%M%S", localtime(&curTime); /* 格式化当前时间 */ \
	\
	USER_PRINT_BASE("[%s %s----line %d]%s"format, timeStr, __FILE__, __LINE, "   ", ##args); \
}


#endif
