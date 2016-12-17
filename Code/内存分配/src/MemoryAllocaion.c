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

SLL g_pLLMemList[MEM_BLOCK_NUM] = NULL;



/****************************************************************************
   function name :      MEM_matchListIndexbyLevel
           input :           
          output :		
    return value :      输出匹配的级别
         history :      
               1 :      2016-12-17 created by xueyu
                        根据数据块大小匹配其级别
                        
****************************************************************************/
ULONG  MEM_matchListIndexbyLevel(ULONG ulBlockLevel)
{
    switch (ulBlockLevel)
    {
        case LL_ZERO:
            return 0;

        case LL_ONE:
            return 1;

        case LL_TWO:
            return 2;

        case LL_THREE:
            return 3;

        case LL_FOUR:
            return 4;

        case LL_FIVE:
            return 5;

        default:
            return NULL_ULONG; 
    }
}



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
        SLL_ADD(pList, pNode, false) 

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
    /* 上面那个注释就是扯淡，最大只能是LL_FIVE，也就是最大的链表节点大小（2016年12月17日） */
   
    /* 初始化链表 */
    SLL_INIT(&g_pLLMemList[0])
    SLL_INIT(&g_pLLMemList[1])
    SLL_INIT(&g_pLLMemList[2])
    SLL_INIT(&g_pLLMemList[3])
    SLL_INIT(&g_pLLMemList[4])
    SLL_INIT(&g_pLLMemList[5])

    /* 将内存拆开挂到链表上，先挂满大链表的再往小的挂 */
    ulTmpSize = LL_FIVE;

    /* 这里现在是有问题的，分配空闲空间必须要考虑到是不是伙伴的，其实初始化的时候应该尽可能简单，直接满足最大块的整数倍（2016年12月17日） */
    while (0 != MEM_SIZE / ulTmpSize)
    {
        ulTmpNodeNum = MEM_SIZE / ulTmpSize;
        
        switch (ulTmpSize)
        {
           case LL_FIVE:
                ulMemIndex == MEM_ALLOCA_LLNODE_INIT(ulMemIndex, &g_pLLMemList[5], ulTmpSize,  ulTmpNodeNum ))
                if (NULL_ULONG == ulMemIndex )
                {
                    /* 这里应该做异常处理,包括释放内存等，以后再说吧 */
                    return;
                }
                continue;
   
           case LL_FOUR:
                ulMemIndex == MEM_ALLOCA_LLNODE_INIT(ulMemIndex, g_pLLMemList[4], ulTmpSize,  ulTmpNodeNum ))
                if (NULL_ULONG == ulMemIndex )
                {
                    /* 这里应该做异常处理，以后再说吧 */
                    return;
                }
                continue;

           case LL_THREE:
                ulMemIndex == MEM_ALLOCA_LLNODE_INIT(ulMemIndex, g_pLLMemList[3], ulTmpSize,  ulTmpNodeNum ))
                if (NULL_ULONG == ulMemIndex )
                {
                    /* 这里应该做异常处理，以后再说吧 */
                    return;
                }
                continue;

           case LL_TWO:
                ulMemIndex == MEM_ALLOCA_LLNODE_INIT(ulMemIndex, g_pLLMemList[2], ulTmpSize,  ulTmpNodeNum ))
                if (NULL_ULONG == ulMemIndex )
                {
                    /* 这里应该做异常处理，以后再说吧 */
                    return;
                }
                continue;

           case LL_ONE:
                ulMemIndex == MEM_ALLOCA_LLNODE_INIT(ulMemIndex, g_pLLMemList[1], ulTmpSize,  ulTmpNodeNum ))
                if (NULL_ULONG == ulMemIndex )
                {
                    /* 这里应该做异常处理，以后再说吧 */
                    return;
                }
                continue;

         case LL_ZERO:
                ulMemIndex == MEM_ALLOCA_LLNODE_INIT(ulMemIndex, g_pLLMemList[0], ulTmpSize,  ulTmpNodeNum ))
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
               1 :      2016-12-16 created by xueyu
                        找合适的块大小
                                    
****************************************************************************/
VOID MEM_findMatchAlaBlock(ULONG ulSize, ULONG ulFindSize, ULONG *pulMatchSize)
{
    if (ulFindSize / 2 < ulSzie )
    {
        *pulMatchSize = ulFindSize;
        return;
    }
    else
    {
        findMatchAlaBlock(ulSize, ulFindSize / 2, pulMatchSize);
    }
 
}



/****************************************************************************
   function name :      MEM_findBuddyForBlock
           input :           
          output :		
    return value :
         history :      
               1 :      2016-12-17 created by xueyu
                        为当前的数据块判断是否有空闲伙伴内存块,有则返回buddy的索引
                                    
****************************************************************************/
ULONG MEM_findBuddyForBlock(ULONG ulBloackLevel, ULONG ulBlockIndex)
{   
    ULONG ulTmp = NULL_ULONG;
    ULONG ulBuddyIndex = NULL_ULONG；
    SLL_NODE *pNode = NULL;
    
    /* 先除以对应的数据块level，用于后面判断 */
    ulTmp = ulBloackLevel / ulBloackLevel;

    /* 根据奇偶判断伙伴是在左边还是右边，偶数伙伴在右边。  */
    if (ulTmp % 2 = 0)
    {
        if (pNode = FindNodeByBlockIndex(ulBloackLevel, ulBlockIndex + ulBloackLevel))
        {
            if ( true == pNode->bFree )
            {
               return (ulBlockIndex + ulBloackLevel);
            }
        }
        
        return NULL_ULONG; 
    }
    else
    {
        if (pNode = FindNodeByBlockIndex(ulBloackLevel, ulBlockIndex - ulBloackLevel))
        {
            if ( true == pNode->bFree )
            {
               return (ulBlockIndex - ulBloackLevel);
            }
        }
        
        return NULL_ULONG;     
    }
}


/****************************************************************************
   function name :      MEM_findCanAllocListIndex
           input :      ulNeedListIndex    实际需要的内存链表索引
          output :      ulCanAllocListIndex   能分配内存的链表索引		
    return value :
         history :      
               1 :      2016-12-17 created by xueyu
                        找到能分配的链表索引
                        
****************************************************************************/
ULONG MEM_findCanAllocListIndex(ULONG ulNeedListIndex)
{
    for (ulNeedListIndex; ulNeedListIndex <= 5; ulNeedListIndex++)
    {
        if (g_pLLMemList[ulNeedListIndex])
    }
}




/****************************************************************************
   function name :      MEM_allocBlock
           input :      ulNeedBlockLevel    实际需要的内存大小级别 

          output :		
    return value :
         history :      
               1 :      2016-12-17 created by xueyu
                        申请内存块，一级一级往上找，知道找到可以分配的内存块，再一级一级往下拆，
                        直到拆分出合适的内存块
                        
****************************************************************************/
ULONG MEM_allocBlock(ULONG ulNeedBlockLevel)
{
    ULONG ulListIndex = NULL_ULONG;
    ulListIndex = MEM_matchListIndexbyLevel(ulNeedBlockLevel);
    if (NULL_ULONG == ulListIndex)
    {
        return VOS_FALSE;
    }


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
    ULONG ulAlaBlockSize;  /* 分配的内存块大小 */ 

    if (0 == ulSize || ulSize > MEM_MAX_ALLOCA  )
    {
        return;
    }

    ulSizeTmp  = ulSize;

    /* 大于LL_FIVE就先分LL_FIVE，直到分来小于LL_FIVE
       这里大于了LL_FIVE怎么判断两个LL_FIVE是连续的，似乎无法判断吧 ? */
    while (ulSizeTmp > LL_FIVE)
    {
        /* 这种情况后面再完善吧 */
        return;
    }

    /* size < LL_FIVE 时，分配的时候不需要考虑合并,但是找不到匹配的就要拆开大的 */
    MEM_findMatchAlaBlock(ulSize, LL_FIVE, &ulAlaBlockSize);

    
    



    

}



