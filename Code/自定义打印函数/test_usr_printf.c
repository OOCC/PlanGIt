/*************************************************************************
	> File Name: test_usr_printf.c
	> Author: marvin
	> Mail: marvinkoala@google.com
	> Created Time: Mon 11 Apr 2016 05:51:07 PM CST
 ************************************************************************/
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "type_def.h"
#include "log.h"

char *make_msg(const char *fmt, ...);


int main()
{
    u8 paTest[32] = "aa,,,,aaa,aa,,a,a,a,,a";
    u8 *pcTestStr = "zao qi de chong er neng chi niao!";
    int32 i32TestVal = 23333;


    //PrintBase("%s aaa bbb ccc %s\n", pcTestStr, paTest);

    LogInfo("%p", paTest);
    LogWarn("%s\n", pcTestStr);
    LogError("%d\n", i32TestVal);
    LogInfo("***************************");

    
    printf("\n");


    LogInfo("%p %s %d\n", paTest, pcTestStr, i32TestVal);
    LogWarn("%p %s %d\n", paTest, pcTestStr, i32TestVal);
    LogError("%p %s %d\n", paTest, pcTestStr, i32TestVal);
    

    return 0;
}


char *make_msg(const char *fmt, ...)
{
    int size,n;
    char p[1024]={0};
    va_list ap;


    //p = (char *)malloc(size);
    

    va_start(ap, fmt);
    n=vsnprintf(p, 1024, fmt, ap);
    va_end(ap);


    return NULL;
}
