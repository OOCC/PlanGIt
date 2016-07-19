/* 
    FILE NAME: N_NumFind1.c
    FUNCTION: from n num to find 1 
    Author:luoyefeisong

  */

#include <stdio.h>
#include <stdlib.h>
#include "type_def.h"
#include "N_NumFind1.h"
#include "usr_printf.h"

/* 将ULONG的数字转换为UCHAR的字符串，长度最多为10位 */
UCHAR *ULNumberToUCString(ULONG ulNumber)
{
    char* szBuffer = (char *)malloc(sizeof(int) + 10);  //分配动态内存

    memset(szBuffer, 0, sizeof(int) + 10);              //内存块初始化

    sprintf(szBuffer, "%d", ulNumber);                  //整数转化为字符串
    
    return szBuffer;
    
    //how to free?
}

ULONG Input_NumFind()
{
    ULONG ulInputNum;
    printf("please input the number, the min is 1, max is 4294967294:");

    scanf("%d", (INT32*)&ulInputNum );
    
    return ulInputNum;

}

/* Output Number */
ULONG Output_NumFind(ulOneIsFound)
{
    UsrPrintf("the 1 number is %lu \n",ulOneIsFound);
    return OK;
}

/* 查找数中的1 */
ULONG N_NumFindOne(ULONG ulNum, ULONG *pulOneIsFound)
{
    ULONG ulReviceNum = 0;
    ULONG ulIndex = 0;
    UCHAR ucToString[MAX_STRING_LEN] = {0};
    UCHAR *pucToString = ucToString;
    ULONG ulNumber = 0;
    ULONG ulOneIsFoundStatistics = 0;

    ulReviceNum = ulNum;
    
    UsrPrintf("Start suan, revice number is %lu\n", ulReviceNum);
    
    /* 循环遍历，直到ulIndex达到接收到的数值为止 */
    for (ulIndex; ulIndex <= ulReviceNum; ulIndex++)
    {
        pucToString = ULNumberToUCString(ulIndex);
        //UsrPrintf("the number is %s\n",pucToString);
        
        /* 这里要初始化一下，否则会不断累加，很快超过字符串长度的最大值 */
        ulNumber = 0;
        
        /* 循环找转换成字符串中有没有‘1’ */
        while (0 != *pucToString)
        {
           
            ulNumber++;
            
            //UsrPrintf("The Number is %s\n",pucToString);
            /* 避免超过字符串长度的最大值，也就是ULONG数字变为字符串最多就十个字符 */
            if (ulNumber > MAX_STRING_LEN)
            {
                UsrPrintf("String is too long!\n");
                return ERR;
            }

            /* 如果是‘1’ */
            if (DEST_STRING == *pucToString)
            {
                ulOneIsFoundStatistics++;
            }
            
            pucToString++;
        } 
    }
    UsrPrintf("End suan\n");

    *pulOneIsFound =  ulOneIsFoundStatistics;
    return OK;
}




