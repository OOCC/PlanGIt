/*******************************************************************************
   文件名     : MemoryAllcation.c
   
   修改历史   :    
   1.  时间 :        2016-12-09  created by luoyefeisong
       内容 :        内存分配与管理，采用伙伴算法。首先模拟最简单的malloc
   2.  时间 :        
       内容 :
****************************************************************************/



#include <stdio.h>
#include <stdbool.h>
#include "MemoryAllocation.h"

/* 以下的绝对路径后面改为makfile */
#include "../include/public_headfile/type_def.h"
#include "../include/public_headfile/public_def.h"                       


ULONG g_ulMemIndex = 0;
SLL_NODE  g_MemoryCtl[MEM_SIZE] = {0};   /* 控制块，这里不应该有MEM_SIZE那么大了 */
CHAR      g_MemoryData[MEM_SIZE] = {0};  /* ���ݿ飬Ҳ���Ƿָ��û�ʹ�õ��ڴ�顣���ݿ�Ϳ��ƿ��index��һһ��Ӧ��  */


SLL g_pLLMemList[MEM_BLOCK_NUM] = { 0 };


/****************************************************************************
   function name :      MEM_matchBlockLevelByListIndex
           input :           
          output :		
    return value :      输出匹配的level
         history :      
               1 :      2016-12-17 created by xueyu
                        根据数据块级别匹配链表索引
                        
****************************************************************************/
ULONG  MEM_matchBlockLevelByListIndex(ULONG ulListIndex)
{
    switch (ulListIndex)
    {
        case 0:
            return LL_ZERO;

        case 1:
            return LL_ONE;

        case 2:
            return LL_TWO;

        case 3:
            return LL_THREE;

        case 4:
            return LL_FOUR;

        case 5:
            return LL_FIVE;

        default:
            return NULL_ULONG; 
    }
}



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
ULONG MEM_ALLOCA_LLNODE_INIT(ULONG ulMemIndex, SLL *pList, ULONG ulListIndex, ULONG ulListNum )
{
    ULONG index = 0;
    SLL_NODE *pNode = NULL;

    /* 大于MEM_SIZE就是扯淡，最大值应该算一下的，先这样吧，后面再搞 */
    if (0 == ulListNum || ulListNum > MEM_SIZE )
    {
        return NULL_ULONG;
    }

    for (index = 0; index < ulListNum; index++)
    {
		pNode = &g_MemoryCtl[ulMemIndex];
        SLL_ADD(pList, pNode, true) 

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
VOID MEM_ALLOCA_INIT(VOID)
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
			   ulMemIndex = MEM_ALLOCA_LLNODE_INIT(ulMemIndex, &g_pLLMemList[5], ulTmpSize, ulTmpNodeNum);
                if (NULL_ULONG == ulMemIndex )
                {
                    /* 这里应该做异常处理,包括释放内存等，以后再说吧 */
                    return;
                }
                break;
   
           case LL_FOUR:
			   ulMemIndex = MEM_ALLOCA_LLNODE_INIT(ulMemIndex, &g_pLLMemList[4], ulTmpSize, ulTmpNodeNum);
                if (NULL_ULONG == ulMemIndex )
                {
                    /* 这里应该做异常处理，以后再说吧 */
                    return;
                }
				break;

           case LL_THREE:
			   ulMemIndex = MEM_ALLOCA_LLNODE_INIT(ulMemIndex, &g_pLLMemList[3], ulTmpSize, ulTmpNodeNum);
                if (NULL_ULONG == ulMemIndex )
                {
                    /* 这里应该做异常处理，以后再说吧 */
                    return;
                }
				break;

           case LL_TWO:
			   ulMemIndex = MEM_ALLOCA_LLNODE_INIT(ulMemIndex, &g_pLLMemList[2], ulTmpSize, ulTmpNodeNum);
                if (NULL_ULONG == ulMemIndex )
                {
                    /* 这里应该做异常处理，以后再说吧 */
                    return;
                }
				break;

           case LL_ONE:
			   ulMemIndex = MEM_ALLOCA_LLNODE_INIT(ulMemIndex, &g_pLLMemList[1], ulTmpSize, ulTmpNodeNum);
                if (NULL_ULONG == ulMemIndex )
                {
                    /* 这里应该做异常处理，以后再说吧 */
                    return;
                }
				break;

         case LL_ZERO:
			 ulMemIndex = MEM_ALLOCA_LLNODE_INIT(ulMemIndex, &g_pLLMemList[0], ulTmpSize, ulTmpNodeNum);
                if (NULL_ULONG == ulMemIndex )
                {
                    /* 这里应该做异常处理，以后再说吧 */
                    return;
                }
				break;

           default:
                return; 
        }

		if (ulMemIndex == 10240)
		{
			break;
		}

        /* 从2^5 开始逐步挂载链表，直到全部找完 */
		ulTmpSize = ulTmpSize / 2;
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
    if (ulFindSize / 2 < ulSize)
    {
        *pulMatchSize = ulFindSize;
        return;
    }
    else
    {
        MEM_findMatchAlaBlock(ulSize, ulFindSize / 2, pulMatchSize);
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
    SLL_NODE *pNode = NULL;
    
    /* 先除以对应的数据块level，用于后面判断 */
    ulTmp = ulBlockIndex / ulBloackLevel;

    /* 根据奇偶判断伙伴是在左边还是右边，偶数伙伴在右边。  */
    if ((ulTmp % 2) == 0)
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
          output :      pulCanAllocListIndex   能分配内存的链表索引		
    return value :
         history :      
               1 :      2016-12-17 created by xueyu
                        找到能分配的链表索引和节点
                        
****************************************************************************/
SLL_NODE  *MEM_findCanAllocListIndex(ULONG ulNeedListIndex, ULONG *pulCanAllocListIndex)
{
    SLL_NODE *pNodeTmp = NULL;

    for (; ulNeedListIndex <= 5; ulNeedListIndex++)
    {
        pNodeTmp = FindFreeNodebyListIndex(ulNeedListIndex);
        if (NULL != pNodeTmp)
        {
            *pulCanAllocListIndex = ulNeedListIndex;
            return pNodeTmp;
        }

    }

    /* 一开始就应该限制住，这里不应该会出现这种情况 */
    *pulCanAllocListIndex = NULL_ULONG;
    return NULL;
}


/****************************************************************************
   function name :      MEM_separateNode
           input :      ulListIndex
                        pNode
                        
          output :		ppNodeSmallFree  返回空闲的小节点指针 
    return value :
         history :      
               1 :      2016-12-17 created by xueyu
                        拆开一个NODE，使其成为两个小NODE，并且左边的为空闲，右边的为使用
                        
****************************************************************************/
ULONG MEM_separateNode(ULONG ulListIndex ,SLL_NODE *pNode, SLL_NODE **ppNodeSmallFree)
{
    ULONG ulSmallListIndex = ulListIndex - 1;
    ULONG ulBlockIndex = pNode - &g_MemoryCtl[0];
    ULONG ulBlockLevel = NULL_ULONG;


	ulBlockLevel = MEM_matchBlockLevelByListIndex(ulSmallListIndex);
    if (ulBlockLevel == NULL_ULONG)
    {
        return VOS_ERR;
    }

    /* 先删除大的节点 */
    SLL_DEL(&g_pLLMemList[ulListIndex], pNode)

    /* 再添加小的节点 */
    SLL_ADD(&g_pLLMemList[ulSmallListIndex], &g_MemoryCtl[ulBlockIndex], true)
    SLL_ADD(&g_pLLMemList[ulSmallListIndex], &g_MemoryCtl[ulBlockIndex + ulBlockLevel], false)

    *ppNodeSmallFree = &g_MemoryCtl[ulBlockIndex + ulBlockLevel];

    return VOS_OK;

}


ULONG MEM_combineNode(ULONG ulListIndex, SLL_NODE *pBuddyNode, SLL_NODE *pNode, SLL_NODE **ppCombineNode)
{
    SLL_NODE *pCombineNode = NULL; 
    ULONG ulCombineListIndex = ulListIndex + 1;

    if (NULL == pBuddyNode || NULL == pNode)
    {
        return VOS_ERR;
    }

    if (pBuddyNode > pNode)
    {
        pCombineNode = pNode;
    }
    else
    {
        pCombineNode = pBuddyNode;
    }


    SLL_DEL(&g_pLLMemList[ulListIndex], pNode)
    SLL_DEL(&g_pLLMemList[ulListIndex], pBuddyNode)

    SLL_ADD(&g_pLLMemList[ulCombineListIndex], pCombineNode, true)

    *ppCombineNode = pCombineNode;

    return VOS_OK;
    
    
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
SLL_NODE *MEM_allocBlock(ULONG ulNeedBlockLevel)
{
    SLL_NODE *pNode = NULL;
    SLL_NODE *pNodeTmp = NULL;
    ULONG ulCanAllocListIndex = NULL_ULONG;
    ULONG ulCanAllocListIndexTmp = NULL_ULONG;    
    ULONG ulListIndex = NULL_ULONG;
    SLL_NODE *pNodeSmall = NULL;

    /* 匹配要申请内存size对应的ListIndex */
    ulListIndex = MEM_matchListIndexbyLevel(ulNeedBlockLevel);
    if (NULL_ULONG == ulListIndex)
    {
        return NULL;
    }
    
    /* 找到能够申请内存的节点和ListIndex */
    pNode = MEM_findCanAllocListIndex(ulListIndex, &ulCanAllocListIndex);
    if (NULL == pNode && NULL_ULONG == ulCanAllocListIndex)
    {
        return NULL;
    }

    if (ulListIndex == ulCanAllocListIndex)
    {
        pNode->bFree = false;
        return pNode;
    }
    else
    {
        ulCanAllocListIndexTmp = ulCanAllocListIndex;

        pNodeTmp = pNode;

        while (ulCanAllocListIndexTmp != ulListIndex)
        {
            if (NULL != pNodeSmall)
            {
                pNodeTmp = pNodeSmall;
            }

            (VOID)MEM_separateNode(ulCanAllocListIndexTmp, pNodeTmp, &pNodeSmall);

            if (ulCanAllocListIndexTmp != 0)
            {
				ulCanAllocListIndexTmp--;
            }
            else
            {
                return NULL;
            }
        }
    }

    
    return pNodeSmall;

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
void *malloc_x(ULONG ulSize)
{
    ULONG ulSizeTmp = NULL_ULONG;
    ULONG ulAlaBlockSize = NULL_ULONG;  /* 分配的内存块大小 */ 
    SLL_NODE *pAlaNode = NULL;

    if (0 == ulSize || ulSize > MEM_MAX_ALLOCA  )
    {
        return NULL;
    }

    ulSizeTmp  = ulSize;

    /* 大于LL_FIVE就先分LL_FIVE，直到分来小于LL_FIVE
       这里大于了LL_FIVE怎么判断两个LL_FIVE是连续的，似乎无法判断吧 ? */
    while (ulSizeTmp > LL_FIVE)
    {
        /* 这种情况后面再完善吧 */
        return NULL;
    }

    /* size < LL_FIVE 时，分配的时候不需要考虑合并,但是找不到匹配的就要拆开大的 */
    MEM_findMatchAlaBlock(ulSize, LL_FIVE, &ulAlaBlockSize);

    /* 分配内存 */
    if ( !(pAlaNode = MEM_allocBlock(ulAlaBlockSize) ))
    {
        return NULL;
    }

    return pAlaNode;
}


/****************************************************************************
   function name :      free_x
           input :           
          output :   
    return value :
         history :      
               1 :      2016-12-17 created by xueyu
                        释放内存
                        
****************************************************************************/
ULONG free_x(VOID *p)
{
    SLL_NODE *pNode = NULL;
    SLL_NODE *pBuddyNode = NULL;
	SLL_NODE *pNodeTmp = NULL;
    SLL_NODE *pCombineNode = NULL;
    ULONG ulBloackLevel = NULL_ULONG;
	ULONG ulBlockIndex = NULL_ULONG;
    

    if (NULL == p)
    {
        return VOS_ERR;
    }

    pNode = (SLL_NODE *)p;
    if (pNode->bFree != false || pNode->ulListIndex > 5 || pNode->ulBlockIndex >= MEM_SIZE)
    {
        return VOS_ERR;
    }

    pNodeTmp = pNode;


	while (1)
	{
		if (5 == pNodeTmp->ulListIndex)
		{
			pNodeTmp->bFree = true;
			return VOS_OK;
		}

		ulBloackLevel = MEM_matchBlockLevelByListIndex(pNodeTmp->ulListIndex);
		if (NULL_ULONG == ulBloackLevel)
		{
			return VOS_ERR;
		}

		ulBlockIndex = MEM_findBuddyForBlock(ulBloackLevel, pNodeTmp->ulBlockIndex);

		if (NULL_ULONG == ulBlockIndex)
		{
			pNodeTmp->bFree = true;
			return VOS_OK;
		}

		pBuddyNode = &g_MemoryCtl[ulBlockIndex];

		if (NULL == pBuddyNode)
		{
			return VOS_OK;
		}

		(VOID)MEM_combineNode(pBuddyNode->ulListIndex, pBuddyNode, pNodeTmp, &pCombineNode);

		pNodeTmp = pCombineNode;
	}
	
   
    
    return VOS_ERR;
}


