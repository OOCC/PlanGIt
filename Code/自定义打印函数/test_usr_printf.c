/*************************************************************************
	> File Name: test_usr_printf.c
	> Author: marvin
	> Mail: marvinkoala@google.com
	> Created Time: Mon 11 Apr 2016 05:51:07 PM CST
 ************************************************************************/
#include "type_def.h"
#include "usr_printf.h"

int main()
{
    u8 paTest[32] = "aa,,,,aaa,aa,,a,a,a,,a";
    u8 *pcTestStr = "zao qi de chong er neng chi niao!";
    int32 i32TestVal = 23333;

    
    UsrPrintf("%p\n", paTest);
    UsrPrintf("%s\n", pcTestStr);
    UsrPrintf("%d\n", i32TestVal);
    UsrPrintf("***************************\n");


    UsrPrintf("%p %s %d\n", paTest, pcTestStr, i32TestVal);

    return 0;
}
