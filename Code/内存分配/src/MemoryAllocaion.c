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

ULONG g_ulMemIndex = 0;
CHAR g_cMemory[MEM_SIZE] = 0; 

SLL *g_pLLMemList0 = NULL;
SLL *g_pLLMemList1 = NULL;
SLL *g_pLLMemList2 = NULL;
SLL *g_pLLMemList3 = NULL;
SLL *g_pLLMemList4 = NULL;
SLL *g_pLLMemList5 = NULL;


/****************************************************************************
   function name :      MEM_ALLOCA_LLNODE_INIT
           input :           
          output :		
    return value :
         history :      
               1 :      2016-12-14 created by xueyu
                        初始化内存数据，挂载到链表中
                        
****************************************************************************/
ULONG MEM_ALLOCA_LLNODE_INIT(ulMemIndex, SLL *pList, ULONG ulListIndex, ULONG ulListNum )
{
    ULONG index = 0;

    /* 大于MEM_SIZE就是扯淡，最大值应该算一下的，先这样吧，后面再搞 */
    if (0 == ulListNum || ulListNum > MEM_SIZE )
    {
        return NULL_ULONG;
    }

    for (index = 0; index < ulListNum; index++)
    {
        pNode = &g_cMemory[ulMemIndex];
        SLL_ADD(pList, pNode) 

        ulMemIndex += ulListIndex;
    }

    /* 记录全局MemIndex */
    g_ulMemIndex = ulMemIndex;
    return ulMemIndex;
}



/****************************************************************************
   function name :      MEM_ALLOCA_INIT
           input :           
          output :		
    return value :
         history :      
               1 :      2016-12-14 created by xueyu
                        内存管理初始化函数
                        
****************************************************************************/
VOID MEM_ALLOCA_INIT()
{

    ULONG ulTmpNodeNum = 0;
    ULONG ulTmpSize = 0;
    ULONG ulMemIndex = 0;

    /* 假设最大申请128个字节的连续空间 */

    /* 先初始化6个链表，分别对应2^n大小的空闲空间块 */
    g_pLLMemList0 = (SLL *)malloc(sizeof(SLL));
    g_pLLMemList1 = (SLL *)malloc(sizeof(SLL));
    g_pLLMemList2 = (SLL *)malloc(sizeof(SLL));
    g_pLLMemList3 = (SLL *)malloc(sizeof(SLL));
    g_pLLMemList4 = (SLL *)malloc(sizeof(SLL));
    g_pLLMemList5 = (SLL *)malloc(sizeof(SLL));
 
    if (NULL == g_pLLMemList0 | 
        NULL == g_pLLMemList1 | 
        NULL == g_pLLMemList2 | 
        NULL == g_pLLMemList3 | 
        NULL == g_pLLMemList4 | 
        NULL == g_pLLMemList5)
        {
            print("malloc fail");
            return;
        }

    /* 初始化链表 */
    SLL_INIT(g_pLLMemList0)
    SLL_INIT(g_pLLMemList1)
    SLL_INIT(g_pLLMemList2)
    SLL_INIT(g_pLLMemList3)
    SLL_INIT(g_pLLMemList4)
    SLL_INIT(g_pLLMemList5)

    /* 将内存拆开挂到链表上，先挂满大链表的再往小的挂 */
    ulTmpSize = LL_FIVE;

    while (0 != MEM_SIZE / ulTmpSize)
    {
        ulTmpNodeNum = MEM_SIZE / ulTmpSize;
        
        switch (ulTmpSize)
        {
           case LL_FIVE:
                ulMemIndex == MEM_ALLOCA_LLNODE_INIT(ulMemIndex, g_pLLMemList5, ulTmpSize,  ulTmpNodeNum ))
                if (NULL_ULONG == ulMemIndex )
                {
                    /* 这里应该做异常处理,包括释放内存等，以后再说吧 */
                    return;
                }
                continue;
   
           case LL_FOUR:
                ulMemIndex == MEM_ALLOCA_LLNODE_INIT(ulMemIndex, g_pLLMemList4, ulTmpSize,  ulTmpNodeNum ))
                if (NULL_ULONG == ulMemIndex )
                {
                    /* 这里应该做异常处理，以后再说吧 */
                    return;
                }
                continue;

           case LL_THREE:
                ulMemIndex == MEM_ALLOCA_LLNODE_INIT(ulMemIndex, g_pLLMemList3, ulTmpSize,  ulTmpNodeNum ))
                if (NULL_ULONG == ulMemIndex )
                {
                    /* 这里应该做异常处理，以后再说吧 */
                    return;
                }
                continue;

           case LL_TWO:
                ulMemIndex == MEM_ALLOCA_LLNODE_INIT(ulMemIndex, g_pLLMemList2, ulTmpSize,  ulTmpNodeNum ))
                if (NULL_ULONG == ulMemIndex )
                {
                    /* 这里应该做异常处理，以后再说吧 */
                    return;
                }
                continue;

           case LL_ONE:
                ulMemIndex == MEM_ALLOCA_LLNODE_INIT(ulMemIndex, g_pLLMemList1, ulTmpSize,  ulTmpNodeNum ))
                if (NULL_ULONG == ulMemIndex )
                {
                    /* 这里应该做异常处理，以后再说吧 */
                    return;
                }
                continue;

         case LL_ZERO:
                ulMemIndex == MEM_ALLOCA_LLNODE_INIT(ulMemIndex, g_pLLMemList0, ulTmpSize,  ulTmpNodeNum ))
                if (NULL_ULONG == ulMemIndex )
                {
                    /* 这里应该做异常处理，以后再说吧 */
                    return;
                }
                continue;

           default:
                return; 
        }

        /* 从2^5 开始逐步挂载链表，直到全部找完 */
        ulTmpSize = ulTmpSize / 2
    }

    return;
}

/****************************************************************************
   function name :      malloc_x
           input :           
          output :		
    return value :
         history :      
               1 :      2016-12-14 created by xueyu
                        自定义malloc函数为mlloc_x
                        找到大小最接近的数据块，超过最大则先分最大再往小的分
                        
****************************************************************************/
VOID *malloc_x(ULONG ulSize)
{
    ULONG ulSizeTmp = 0；

    if (0 == ulSize || ulSize > MEM_MAX_ALLOCA  )
    {
        return;
    }

    ulSizeTmp  = ulSize;

    /* 大于LL_FIVE就先分LL_FIVE，直到分来小于LL_FIVE
       这里大于了LL_FIVE怎么判断两个LL_FIVE是连续的，似乎无法判断吧 ? */
    while (ulSizeTmp > LL_FIVE)
    {

    }

    /* size < LL_FIVE 时，递归分配 */



    

}



