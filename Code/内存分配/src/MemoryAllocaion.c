/****************************************************************************
   文件名     : MemoryAllcation.c
   
   修改历史   :    
   1.  时间 :        2016-12-09  created by luoyefeisong
       内容 :        内存分配与管理，采用伙伴算法。首先模拟最简单的malloc
   2.  时间 :        
       内容 :
****************************************************************************/



#include <stdio.h>

#include "MemoryAllcation.h"

/* 以下的绝对路径后面改为makfile */
#include "../type_def.h"
#include "../public_def.h"                       
#include "../include/linklist_pub.h"


CHAR g_cMemory[MEM_SIZE] = 0; 



VOID MEM_ALLOCA_INIT()
{

    ULONG ulTmpNodeNum = 0;
    ULONG ulTmpSize = 0;

    /* 假设最大申请128个字节的连续空间 */

    /* 先初始化6个链表，分别对应2^n大小的空闲空间块 */
    SLL memList_0;
    SLL memList_1;
    SLL memList_2;
    SLL memList_3;
    SLL memList_4;
    SLL memList_5;

    (VOID)memset(&memList_0, 0, sizeof(SLL));
    (VOID)memset(&memList_1, 0, sizeof(SLL));    
    (VOID)memset(&memList_2, 0, sizeof(SLL));    
    (VOID)memset(&memList_3, 0, sizeof(SLL));    
    (VOID)memset(&memList_4, 0, sizeof(SLL));    
    (VOID)memset(&memList_5, 0, sizeof(SLL));    

    /* 将内存拆开挂到链表上，先挂满大链表的再往小的挂 */
    ulTmpSize = LL_FIVE;

    while (0 != MEM_SIZE / ulTmpSize)
    {
        ulTmpNodeNum = MEM_SIZE / ulTmpSize;
        


        ulTmpSize = ulTmpSize / 2
    }


}




