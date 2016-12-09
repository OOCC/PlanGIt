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

void InsertNode(SLL *pList, SLL_NODE *pNode)
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
	}

    pList->pstTail->pNext = pNode;  	/* �Ƚ�pNode���뵽β�ڵ�֮�� */
    pNode->pNext = NULL;            	/* �ٽ�pNode֮����ΪNULL */
    pList->pstTail = pNode;      		/* ����pstTail��ֵ����ΪpNode */
	
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

