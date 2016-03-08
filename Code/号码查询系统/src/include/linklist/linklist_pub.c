/****************************************************************************
   文件名     : linklist_pub.c
   
   修改历史   :    
   1.  修改时间 :        2016-02-28 created by luoyefeisong
       修改内容 :        链表操作函数
   2.  修改时间 :        
       修改内容 :
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
                        将节点插入循环链表尾
    
               2 : 
****************************************************************************/

void InsertNode(SLL *pList, SLL_NODE *pNode)
{
    if (NULL == pList || NULL == pNode)
    {
        //LOG_ERROR();暂未实现
        return;
    }
    
    pList->ulNodeNum++;
    pNode->pNext = pList->pstTail;
    pList->pstTail = pNode;

    return;
}




