/*************************************************************************
	> File Name: usrPrintf.h
	> Author: marvin
	> Mail: marvinkoala@google.com
	> Created Time: Mon 11 Apr 2016 05:33:34 PM CST
 ************************************************************************/

#ifndef _USR_PRINTF_H
#define _USR_PRINTF_H

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "type_def.h"


#define TIME_STR_LEN                         64
#define LOG_TIME_INFO_FMT                    "[%s]\033[0;32m[INFO]\033[0m    "  //gernal log info fmt (color: green)
#define LOG_TIME_WARN_FMT                    "[%s]\033[0;31m[WARN]\033[0m    "  //warn log info fmt (color: yellow)
#define LOG_TIME_ERROR_FMT                   "[%s]\033[1;33m[ERROR]\033[0m   "  //error log info fmt (color: red)
#define LOG_FILE_LINE_FMT                    "   (%s ---line:%d)"               //file name and line num fmt


//func: check CRLF of the usr format string ,and delete it.
//in:
//out:
//ret:
//note:
int32 CheckCRLFAndDelete(int8 *pFmt)
{
    int8 *pUsrFmtStr;
    int32 usrFmtOffset;
    int32 usrFmtLen;


	pUsrFmtStr = pFmt; 
	usrFmtLen = strlen(pUsrFmtStr);
	

    //check user input format string
    for (usrFmtOffset=0; usrFmtOffset<usrFmtLen; usrFmtOffset++)
    {
		//delete CLRF of user format stirng
        if(pUsrFmtStr[usrFmtOffset] == '\n' && pUsrFmtStr[usrFmtOffset+1] == '\0')
        {
			//printf("offset:%d\n", usrFmtOffset);
			pUsrFmtStr[usrFmtOffset]='\0';
			break;
        }
    }


    return 0;
}


//func: print msg to the stdout. 
//in:
//out:
//ret:
//note: this function will delete CRLF of the usr format string
int32 PrintBase(int8 *format, ...)
{
	int32   i32PrintfArgsNum;
	int32	i32FormatLen;


	//create tmp format string
	i32FormatLen = strlen(format);
	int8 *pcFmt = (int8 *)malloc(i32FormatLen+1);
	strncpy(pcFmt, format, i32FormatLen+1);


	//chech user format string and delete CRLF of this string
	CheckCRLFAndDelete(pcFmt);


	va_list vArgList; //定义一个va_list型的变量,这个变量是指向参数的指针.
	va_start (vArgList, pcFmt); /*用va_start宏初始化变量,这个宏的第二个参数是第一个可变参数的前一个参数,是一个固定的参数.*/
	i32PrintfArgsNum=vprintf(pcFmt, vArgList); //windows function: _vprintf
	va_end(vArgList); //用va_end宏结束可变参数的获取
  

	free(pcFmt);


	return i32PrintfArgsNum; //返回参数的字符个数
}
#define PRINT_LOG_BASE(format, args...)      PrintBase(format, ##args) //define macro of PrintBase for pass uncertain args


//func: Print gernal log info 
//in:
//out:
//ret:
//note: every log msg will auto add CRLF to the format string 
#define LogInfo(format, args...) \
{ \
	int8  timeStr[TIME_STR_LEN]={0}; \
    int8  tmpStr[TIME_STR_LEN]={0};\
	int32 timeStrLen; \
    int32 ms, us; \
    time_t curTime; \
    struct timeval sUs;\
	\
	time(&curTime); \
	timeStrLen=sizeof(timeStr); \
    strftime(timeStr,  timeStrLen, "%Y-%m-%d %H:%M:%S", localtime(&curTime));  \
    \
    gettimeofday(&sUs, NULL);\
    us = (sUs.tv_usec/100%10)*100+(sUs.tv_usec/10%10)*10+sUs.tv_usec%10; \
    ms = (sUs.tv_usec/100000%10)*100+(sUs.tv_usec/10000%10)*10+sUs.tv_usec/1000%10; \
    sprintf(tmpStr, "%s %d.%d", timeStr, ms, us); \
    \
	printf(LOG_TIME_INFO_FMT, tmpStr); \
    PRINT_LOG_BASE(format , ##args); \
    printf(LOG_FILE_LINE_FMT, __FILE__, __LINE__); \
    printf("\n"); \
}


//func: Print warn info  
//in:
//out:
//ret:
//note:
#define LogWarn(format, args...) \
{ \
	int8  timeStr[TIME_STR_LEN]={0}; \
    int8  tmpStr[TIME_STR_LEN]={0};\
	int32 timeStrLen; \
    int32 ms, us; \
    time_t curTime; \
    struct timeval sUs;\
	\
	time(&curTime); \
	timeStrLen=sizeof(timeStr); \
    strftime(timeStr,  timeStrLen, "%Y-%m-%d %H:%M:%S", localtime(&curTime));  \
	\
    gettimeofday(&sUs, NULL);\
    us = (sUs.tv_usec/100%10)*100+(sUs.tv_usec/10%10)*10+sUs.tv_usec%10; \
    ms = (sUs.tv_usec/100000%10)*100+(sUs.tv_usec/10000%10)*10+sUs.tv_usec/1000%10; \
    sprintf(tmpStr, "%s %d.%d", timeStr, ms, us); \
	\
	printf(LOG_TIME_WARN_FMT, tmpStr); \
    PRINT_LOG_BASE(format , ##args); \
    printf(LOG_FILE_LINE_FMT, __FILE__, __LINE__); \
    printf("\n"); \
}


//func: Print error info 
//in:
//out:
//ret:
//note:
#define LogError(format, args...) \
{ \
	int8  timeStr[TIME_STR_LEN]={0}; \
    int8  tmpStr[TIME_STR_LEN]={0};\
	int32 timeStrLen; \
    int32 ms, us; \
    time_t curTime; \
    struct timeval sUs;\
	\
	time(&curTime); \
	timeStrLen=sizeof(timeStr); \
    strftime(timeStr,  timeStrLen, "%Y-%m-%d %H:%M:%S", localtime(&curTime));  \
	\
    gettimeofday(&sUs, NULL);\
    us = (sUs.tv_usec/100%10)*100+(sUs.tv_usec/10%10)*10+sUs.tv_usec%10; \
    ms = (sUs.tv_usec/100000%10)*100+(sUs.tv_usec/10000%10)*10+sUs.tv_usec/1000%10; \
    sprintf(tmpStr, "%s %d.%d", timeStr, ms, us); \
	\
	printf(LOG_TIME_ERROR_FMT, tmpStr); \
    PRINT_LOG_BASE(format , ##args); \
    printf(LOG_FILE_LINE_FMT, __FILE__, __LINE__); \
    printf("\n"); \
}


#endif
