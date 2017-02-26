#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include "public_def.h"
#include "type_def.h"


#define  MAX_BACKTRACE_NUM  100
#define  MAX_BACKTRACE_STRING_SIZE  20

ULONG g_ulBacktraceErr = 0;

ULONG backtraceLog()
{
    void *backtraceArry[MAX_BACKTRACE_NUM] = {0};
    ULONG backtraceRetNum = 0;
    char *backtraceString[MAX_BACKTRACE_STRING_SIZE] = {0};
    ULONG ulIndex = 0;
    char **pBack = (char **)backtraceString;	


    backtraceRetNum = backtrace(backtraceArry, MAX_BACKTRACE_NUM);
    pBack = backtrace_symbols(backtraceArry, backtraceRetNum);
    if (NULL == pBack)
    {
        return VOS_ERR;
    }

    for (ulIndex = 0; ulIndex < backtraceRetNum; ulIndex++)
    {
        printf("address: %s\n", pBack[ulIndex]);
    }

    free(pBack);
    return VOS_OK;
}


void func1()
{
    if (VOS_OK != backtraceLog())
    {
        g_ulBacktraceErr++;
    }
}

void func2()
{
    func1();
}

void func3()
{
    func2();
}


int main()
{
    func3();
}
