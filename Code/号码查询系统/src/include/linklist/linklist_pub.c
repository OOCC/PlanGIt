/****************************************************************************
   �ļ���     : linklist_pub.c
   
   �޸���ʷ   :    
   1.  �޸�ʱ�� :        2016-02-28 created by luoyefeisong
       �޸����� :        �����������
   2.  �޸�ʱ�� :        
       �޸����� :
****************************************************************************/
#include <stdio.h>
#include "linklist_pub.h"
#include "public_def.h"
#include "type_def.h"


/****************************************************************************
   function name :      InsertNode
           input :      pNode     
          output :		pList
    return value :
         history :      
               1 :      2016-02-27 created by luoyefeisong
                        ���ڵ����ѭ������β
    
               2 : 
****************************************************************************/

void InsertNode(SLL *pList, SLL_NODE *pNode)
{
    if (NULL == pList || NULL == pNode)
    {
        //LOG_ERROR();��δʵ��
        return;
    }
    
    pList->ulNodeNum++;
    pNode->pNext = pList->pstTail;
    pList->pstTail = pNode;

    return;
}




