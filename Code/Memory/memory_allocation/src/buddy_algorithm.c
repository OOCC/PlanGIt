/*******************************************************************************
   文件名     : buddy_algorithm.c
   
   修改历史   :    
   1.  时间 :        2018-6-4  created by luoyefeisong
       内容 :        内存分配与管理，采用伙伴算法。首先模拟最简单的malloc
   2.  时间 :        
       内容 :
****************************************************************************/



#include <stdio.h>
#include <stdbool.h>
#include "type_def.h"
#include "public_def.h"    
#include "linklist_pub.h"
#include "buddy_algorithm.h"
#include "bitmap.h"



/* 全局变量 */
SLL   g_pLLMemList[MAX_LEVEL+1]   = {0};
char  g_MemData[MEM_SIZE*1024]    = {0};  /* 仅仅做测试使用 */
char *g_MemoryData                = g_MemData;  /* 这里写需要初始化的内存地址 */
unsigned int  *g_bmp[MAX_LEVEL+1]          = {0};   


/* 初始化位图，根据内存的分配级别及内存总大小进行初始化，每个级别对应一个位图，目前最高level是10 */
void bmp_init()
{
    int i = 0;
    int request_size = 1024*1024;
    
    for (i = 0; i <= MAX_LEVEL+1; i++) {
        /* level 0时，每个数据块占2^0kb的大小，总共需要1G/32个int */
        bmp_create(&g_bmp[i], request_size); 
        request_size /= 2;
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
ULONG MEM_ALLOCA_LLNODE_INIT(ULONG ulMemIndex, SLL *pList, ULONG level, ULONG ulListNum )
{
    ULONG index = 0;
    SLL_NODE *pNode = NULL;

    for (index = 0; index < ulListNum; index++)
    {
        /* ulMemIndex是偏移量，表示当前初始化到内存空间的哪个位置了 */
        pNode = g_MemoryData + ulMemIndex;
        
		SLL_ADD(pList, pNode, 0);
        //bmp_set_bit(g_bmp[level], BMP_OFFSET(pNode, level));

        ulMemIndex += (1<<level)*1024; 
    }
    
    /* 返回ulMemIndex,用于剩余数据空间的初始化 */
    return ulMemIndex;
}



/****************************************************************************
   function name :      mem_alloc_init
           input :           
          output :		
    return value :
         history :      
               1 :      2016-12-14 created by xueyu
                        内存管理初始化函数
                        
****************************************************************************/
VOID MEM_ALLOC_INIT(VOID)
{

    ULONG ulTmpNodeNum = 0;
    ULONG ulMemIndex = 0;
    int i = 0;
    int level = 0;
    
    /* 初始化链表 */
    while (i <= MAX_LEVEL + 1) {
        SLL_INIT(&g_pLLMemList[i]);
        i++;
    }

    /* 将内存拆开挂到链表上，先挂满大链表的再往小的挂 */
    level = MAX_LEVEL;

    /* 这里现在是有问题的，分配空闲空间必须要考虑到是不是伙伴的，其实初始化的时候应该尽可能简单，直接满足最大块的整数倍（2016年12月17日） */
    while (0 != MEM_SIZE / (1<<level) ) 
    {
        ulTmpNodeNum = MEM_SIZE / (1<<level);
        
        ulMemIndex = MEM_ALLOCA_LLNODE_INIT(ulMemIndex, &g_pLLMemList[level], level, ulTmpNodeNum);
 
        if (ulMemIndex >= MEM_SIZE*1024)
        {
            break;
        }

        /* 从2^10 开始逐步挂载链表，直到全部找完 */
        level--;
    }

	bmp_init();

    return;
}


/****************************************************************************
   function name :      calc_mem_level
           input :           
          output :		
    return value :
         history :      
               1 :      2018-6-4 created by xueyu
                        通过size计算申请所需内存大小的级别
****************************************************************************/

int calc_mem_level(ULONG size)
{
    int level = 0;
        
    while (level <= MAX_LEVEL) {
		if (size <= (1 << level)*1024 )
			return level;
        level++;
    }
    
    return NULL_ULONG;
}



/****************************************************************************
   function name :      MEM_separateNode
           input :      ulListIndex
                        pNode
                        
          output :		ppNodeSmallFree  返回空闲的小节点指针 
    return value :
         history :      
               1 :      2016-12-17 created by xueyu
                        拆开一个NODE，使其成为两个小NODE，并且左边的为使用，右边的为空闲
                        
****************************************************************************/
ULONG MEM_separateNode(ULONG ulListLevel, SLL_NODE *pNode)
{
    ULONG ulSmallLevel = ulListLevel - 1;
    
    /* 先删除大的节点 */
    SLL_DEL(&g_pLLMemList[ulListLevel], pNode);
    bmp_clear_bit(g_bmp[ulListLevel], BMP_OFFSET(pNode, ulListLevel));
    
    /* 再添加小的节点 */
   	SLL_ADD(&g_pLLMemList[ulSmallLevel], pNode, 1);
  	SLL_ADD(&g_pLLMemList[ulSmallLevel], (char *)pNode + (1 << ulSmallLevel) * 1024, 0);
    bmp_set_bit(g_bmp[ulSmallLevel], BMP_OFFSET(pNode, ulSmallLevel));
    
    return VOS_OK;

}


/****************************************************************************
   function name :      MEM_findCanAllocListIndex
           input :      ulNeedListIndex    实际需要的内存链表索引
          output :      pulCanAllocLevel   能分配内存的链表索引		
    return value :
         history :      
               1 :      2016-12-17 created by xueyu
                        找到能分配的链表索引和节点
                        
****************************************************************************/
SLL_NODE  *MEM_findCanAllocLevel(int needLevel, ULONG *pulCanAllocLevel)
{
    SLL_NODE *pNodeTmp = NULL;

    for (; needLevel <= MAX_LEVEL; needLevel++)
    {
        pNodeTmp = FindFreeNodebyLevel(needLevel);
        if (NULL != pNodeTmp)
        {
            *pulCanAllocLevel = needLevel;
            return pNodeTmp;
        }
    }

    /* 一开始就应该限制住，这里不应该会出现这种情况 */
    *pulCanAllocLevel = NULL_ULONG;
    return NULL;
}


/****************************************************************************
   function name :      MEM_allocBlock
           input :      ulNeedBlockLevel    实际需要的内存大小级别 

          output :		
    return value :
         history :      
               1 :      2016-12-17 created by xueyu
                        申请内存块，一级一级往上找，直到找到可以分配的内存块，再一级一级往下拆，
                        直到拆分出合适的内存块
                    
****************************************************************************/
SLL_NODE *MEM_allocBlock(int ulNeedBlockLevel)
{
    SLL_NODE *pNode = NULL;
    ULONG ulCanAllocLevel = NULL_ULONG;
    ULONG ulCanAllocLevelTmp = NULL_ULONG;    
    SLL_NODE *pNodeSmall = NULL;
    
    /* 找到能够申请内存的节点和ListIndex */
    pNode = MEM_findCanAllocLevel(ulNeedBlockLevel, &ulCanAllocLevel);
    if (NULL == pNode && NULL_ULONG == ulCanAllocLevel)
    {
        /* 没有可以分配的内存 */
        return NULL;
    }

    if (ulNeedBlockLevel == ulCanAllocLevel)
    {
        bmp_set_bit(g_bmp[ulCanAllocLevel], BMP_OFFSET(pNode, ulCanAllocLevel));
        pNode->used = 1;
        return pNode;
    }
    else
    {
        ulCanAllocLevelTmp = ulCanAllocLevel;
        
        /* 如果level不能等于合适的level，就分裂           */
        while (ulCanAllocLevelTmp != ulNeedBlockLevel)
        {
            (VOID)MEM_separateNode(ulCanAllocLevelTmp, pNode);

            if (ulCanAllocLevelTmp != 0)
            {
                ulCanAllocLevelTmp--;
            }
            else
            {
                return NULL;
            }
        }
    }

    return pNode;

}



/****************************************************************************
   function name :      malloc_x
           input :           
          output :   
    return value :
         history :      
               1 :      2016-12-14 created by xueyu
                        自定义malloc函数为malloc_x
                        找到大小最接近的数据块，超过最大则先分最大再往小的分
                        
****************************************************************************/
void *malloc_x(ULONG ulSize)
{
    ULONG ulAlloclevel = 0;  
    SLL_NODE *pNode = NULL;

    if (0 == ulSize || ulSize > (1<<MAX_LEVEL)*1024 )
    {
        return NULL;
    }

    /* size < MAX_LEVEL 时，分配的时候不需要考虑合并,但是找不到匹配的就要拆开大的 */
    ulAlloclevel = calc_mem_level(ulSize);
	if (NULL_ULONG == ulAlloclevel)
		return NULL;

    /* 分配内存 */
    if ( !(pNode = MEM_allocBlock(ulAlloclevel) ))
    {
        return NULL;
    }

    return pNode;
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
SLL_NODE *MEM_findBuddyForBlock(ULONG ulBloackLevel, SLL_NODE * pNode)
{   
    ULONG offset = NULL_ULONG;
    SLL_NODE *pBuddy = NULL;

    
    /* 先除以对应的数据块level，用于后面判断 */
    offset = BMP_OFFSET(pNode, ulBloackLevel);

    /* 根据奇偶判断伙伴是在左边还是右边，偶数伙伴在右边。  */
    if ((offset % 2) == 0)
        pBuddy = (char *)pNode + (1<<ulBloackLevel)*1024;
    else
        pBuddy = (char *)pNode - (1<<ulBloackLevel)*1024;
   
    if (pBuddy == FindNode(ulBloackLevel, pBuddy))
    {
        if ( false == bmp_get_bit(g_bmp[ulBloackLevel], BMP_OFFSET(pBuddy, ulBloackLevel)) )
        {
           return pBuddy;
        }
    }
    
    return NULL; 
}



ULONG MEM_combineNode(ULONG level, SLL_NODE *pBuddyNode, SLL_NODE *pNode, SLL_NODE **ppCombineNode)
{
    SLL_NODE *pCombineNode = NULL; 
    ULONG ulCombineLevel = level + 1;

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


    SLL_DEL(&g_pLLMemList[level], pNode);
    bmp_clear_bit(g_bmp[level], BMP_OFFSET(pNode, level));
    SLL_DEL(&g_pLLMemList[level], pBuddyNode);
    bmp_clear_bit(g_bmp[level], BMP_OFFSET(pNode, level));

    SLL_ADD(&g_pLLMemList[ulCombineLevel], pCombineNode, 0);
    bmp_set_bit(g_bmp[ulCombineLevel], BMP_OFFSET(pCombineNode, ulCombineLevel));
    
    *ppCombineNode = pCombineNode;

    return VOS_OK;
    
    
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
    SLL_NODE *pCombineNode = NULL;
	ULONG ulBlockIndex = NULL_ULONG;
    ULONG level = 0;
    int used = false;
    
    if (NULL == p)
       return VOS_ERR;

    pNode = (SLL_NODE *)p;

    level = FindLevelByNode(pNode);
    if (level == NULL_ULONG)
        return VOS_ERR;
    
    used = bmp_get_bit(g_bmp[level], BMP_OFFSET(pNode, level)); 
    if (false == used)
        return VOS_ERR;

    while (1)
    {
        if (MAX_LEVEL == level)
        {
        	bmp_clear_bit(g_bmp[level], BMP_OFFSET(pNode, level));
        	return VOS_OK;
        }

        pBuddyNode = MEM_findBuddyForBlock(level, pNode);

        if (NULL == pBuddyNode)
        {
			bmp_clear_bit(g_bmp[level], BMP_OFFSET(pNode, level));
			return VOS_OK;
        }

        (VOID)MEM_combineNode(level, pBuddyNode, pNode, &pCombineNode);

        pNode = pCombineNode;
        level++;
	}
	
   
    
    return VOS_ERR;
}


