/****************************************************************************
   �ļ���     : linklist_pub.c
   
   �޸���ʷ   :    
   1.  �޸�ʱ�� :        2016-02-28 created by luoyefeisong
       �޸����� :        ������������
   2.  �޸�ʱ�� :        
       �޸����� :
****************************************************************************/
#include <stdio.h>
#include "linklist_pub.h"
#include "public_def.h"
#include "type_def.h"





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
    SLL_NODE *pLast = NULL;
    
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
SLL_NODE *FindFreeNodebyListIndex(ULONG ulListIndex)
{
    SLL_NODE *pNodeTmp = NULL;
    
    pNodeTmp = (g_pLLMemList[ulListIndex].stHead)->pNext;

    while (NULL != pNodeTmp)
    {
        if (true == pNodeTmp->bFree)
        {
            return pNodeTmp;
        }
    }

    return NULL;
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
SLL_NODE *FindNodeByBlockIndex(ULONG ulBlockLevel, ULONG ulBlockIndex)
{
    SLL *pLL = NULL;
    SLL_NODE *pTmpNode = NULL;

    pLL = &g_pLLMemList[ulBlockLevel];
    pTmpNode = &pLL->stHead;

    while (NULL != pTmpNode)
    {
        if (pTmpNode->ulBlockIndex == ulBlockIndex)
        {
            return pTmpNode;
        }

        pTmpNode = pTmpNode->pNext;
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

void InsertNode(SLL *pList, SLL_NODE *pNode, bool bFree)
{
    if (NULL == pList || NULL == pNode)
    {
        //LOG_ERROR();��δʵ��
        return;
    }
    
    pList->ulNodeNum++;
    
	/* ��һ���ڵ� */
	if (pList->stHead.pNext == NULL)
	{
		pList->stHead.pNext = pNode;
		pList->pstTail = pNode;
    	pNode->pNext = NULL;

        /* 填数据 */
        pNode->ulBlockIndex = pNode - &g_cMemory[0];
        pNode->bFree = bFree;
	}

    pList->pstTail->pNext = pNode;  	/* �Ƚ�pNode���뵽β�ڵ�֮�� */
    pNode->pNext = NULL;            	/* �ٽ�pNode֮����ΪNULL */
    pList->pstTail = pNode;      		/* ������pstTail��ֵ����ΪpNode */

    /* 填数据 */
    pNode->ulBlockIndex = pNode - &g_cMemory[0];
    pNode->bFree = bFree;

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
    
    pLast == FindNodeByList(pList, pNode);
    if (NULL == pLast)
    {
        return;
    }
    
    pList->ulNodeNum--;  /* 能找到指定节点的位置，说明链表节点数量一定可以自减，不用先判断是否等于0 */
    
    /* 如果要删除的是尾节点 */
    if (NULL == pNode->pNext)
    {
        pList->pstTail = pLast;
    }  
        
    
    /* 先连后断，因为先断了就找不到位置了 */
    pLast->pNext = pNode->pNext;
    pNode->pNext = NULL;
       
    return;
}

