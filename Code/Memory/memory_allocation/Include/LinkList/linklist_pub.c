/****************************************************************************
   �ļ���     : linklist_pub.c
   
   �޸���ʷ   :    
   1.  �޸�ʱ�� :        2016-02-28 created by luoyefeisong
       �޸����� :        ������������
   2.  �޸�ʱ�� :        
       �޸����� :
****************************************************************************/
#include <stdio.h>
#include <stdbool.h>

#include "public_def.h"
#include "type_def.h"
#include "linklist_pub.h"
#include "buddy_algorithm.h"
#include "bitmap.h"

/****************************************************************************
   function name :      FindNodeByList
           input :      pList
                        pNode     
          output :		pLast
    return value :
         history :      
               1 :      2016-12-08 created by xueyu
                        遍历链表，找到指定节点的上一个节点地址（为了拿来删节点）                        
****************************************************************************/
SLL_NODE *FindNodeByList(SLL *pList, SLL_NODE *pNode)
{
    SLL_NODE *pTmpNode = NULL;
    
    if (NULL == pList || NULL == pNode)
    {
        return NULL;
    }
    
    pTmpNode = &(pList->stHead); 
    
    /* 循环找，因为里面会预取下一个，下一个如果为空就不用找了 */
    while (NULL != pTmpNode->pNext)
    {
        /* 预取下一个节点地址来与pNode进行对比 */
        if (pTmpNode->pNext == pNode)
        {
            return pTmpNode;
        }
        pTmpNode = pTmpNode->pNext;
    }
    
    return NULL;
}



/****************************************************************************
   function name :      FindFreeNodebyListIndex
           input :                                   
          output :		
    return value :
         history :      
               1 :      2016-12-17 created by xueyu
                        遍历链表，通过链表index找到空闲节点
*****************************************************************************/
SLL_NODE *FindFreeNodebyLevel(int level)
{
    SLL_NODE *pNodeTmp = NULL;
    pNodeTmp = g_pLLMemList[level].stHead.pNext;

    while (NULL != pNodeTmp)
    {
        if (false == bmp_get_bit(g_bmp[level], BMP_OFFSET(pNodeTmp, level)) ) /* bmp值为1表示内存空间可以使用 */
        {
            return pNodeTmp;
        }
        pNodeTmp = pNodeTmp->pNext;
    }

    return NULL;
}

ULONG FindLevelByNode(SLL_NODE *pNode)
{
    SLL_NODE *pNodeTmp = NULL;
    int level = MAX_LEVEL;
    pNodeTmp = g_pLLMemList[level].stHead.pNext;

    while (level--) 
    {
        pNodeTmp = g_pLLMemList[level].stHead.pNext;
        
        while (NULL != pNodeTmp)
        {
            if (pNodeTmp == pNode)
            {
                return level;
            }
            pNodeTmp = pNodeTmp->pNext;
        }
    }
    return NULL_ULONG;
    
}



/****************************************************************************
   function name :      FindNodeByList
           input :      pList
                        pNode     
          output :		pLast
    return value :
         history :      
               1 :      2016-12-08 created by xueyu
                        遍历链表，通过index找到对应level的节点                        
****************************************************************************/
SLL_NODE *FindNode(ULONG level, SLL_NODE *pNode)
{
    SLL_NODE *pNodeTmp = NULL;
    pNodeTmp = g_pLLMemList[level].stHead.pNext;

    while (NULL != pNodeTmp)
    {
        if (pNode == pNodeTmp) /* bmp值为1表示内存空间可以使用 */
        {
            return pNodeTmp;
        }
        pNodeTmp = pNodeTmp->pNext;
    }

    return NULL;
}




/****************************************************************************
   function name :      InsertNode
           input :      pNode     
          output :		pList
    return value :
         history :      
               1 :      2016-02-27 created by luoyefeisong
                        ���ڵ�����ѭ������β
    
               2 :      2016-12-08 modify by xueyu
                        修改为单向链表
****************************************************************************/

void InsertNode(SLL *pList, SLL_NODE *pNode, char used)
{
    ULONG ulListIndex = NULL_ULONG;

    if (NULL == pList || NULL == pNode)
    {
        //LOG_ERROR();��δʵ��
        return;
    }
    
    pList->ulNodeNum++;
    
	/* 这里不能用head来判断，因为head指向了g_pLLMemList的首地址，是有值的 */
	if (pList->pstTail == NULL)
	{
		pList->stHead.pNext = pNode;
		pList->pstTail = pNode;
    	pNode->pNext = NULL;
	}
    else
    {
        pList->pstTail->pNext = pNode;  	/* �Ƚ�pNode���뵽β�ڵ�֮�� */
        pNode->pNext = NULL;            	/* �ٽ�pNode֮����ΪNULL */
        pList->pstTail = pNode;      		/* ������pstTail��ֵ����ΪpNode */
    }

    pNode->used = used;
    return;
}




/****************************************************************************
   function name :      DeleteNode
           input :      pNode     
          output :		pList
    return value :
         history :      
               1 :      2016-12-08 created by xueyu
                        删除链表中指定的节点
                        
****************************************************************************/

void DeleteNode(SLL *pList, SLL_NODE *pNode)
{
    SLL_NODE *pLast = NULL;
    
    if (NULL == pList || NULL == pNode)
    {
        return;
    }
    
    pLast = FindNodeByList(pList, pNode);
    if (NULL == pLast)
    {
        return;
    }
     
    pList->ulNodeNum--;  /* 能找到指定节点的位置，说明链表节点数量一定可以自减，不用先判断是否等于0 */
    
    /* 如果要删除的是尾节点 */
    if (NULL == pNode->pNext)
    {
        pList->pstTail = pLast;
       
        if (pList->pstTail == &pList->stHead)
            pList->pstTail = NULL;
    }   

    /* 如果删除的是首节点 */
    if (pNode == pList->stHead.pNext)
    {
        pList->stHead.pNext = pNode->pNext;
    }    
    
    /* 先连后断，因为先断了就找不到位置了 */
    pLast->pNext = pNode->pNext;
    pNode->pNext = NULL;
	pNode->used = 0;

    return;
}

