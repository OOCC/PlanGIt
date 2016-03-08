/****************************************************************************
   文件名     : linklist_pub.h
   
   修改历史   :    
   1.  修改时间 :        2016-02-28 created by luoyefeisong
       修改内容 :        链表操作函数头文件
   2.  修改时间 :        
       修改内容 :
****************************************************************************/
//#include "/TEST_LinkList/"

#include <stdio.h>
#include "public_def.h"
#include "type_def.h"
    
typedef struct node {
    struct node  *pNext;
    ULONG    ulData; /* 数据域，只用来存放数据 */
}SLL_NODE;


/* 链表结构体 */
typedef struct {
    SLL_NODE stHead;
    SLL_NODE *pstTail;
    ULONG    ulNodeNum;
}SLL;


#define SLL_INIT(pList) {\
    ((SLL *)pList)->pstTail = &((SLL *)pList)->stHead;\
    ((SLL *)pList)->ulNodeNum = 0;\
}

/*#define SLL_COUNT(pList) {\
    ((SLL *)pList)->ulNodeNum;\
}*/
#define SLL_COUNT(pList) ((SLL *)pList)->ulNodeNum

#define SLL_ADD(pList, pNode) {\
    (void)InsertNode(pList, pNode);\
}

 


#define SLL_NODE_INIT(pNode) {\
    ((SLL_NODE *)pNode)->pNext = NULL;\
    ((SLL_NODE *)pNode)->ulData = 0;\
} 

/*#define SLL_NODE_GET_DATA(pNode) ({\
    ((SLL_NODE *)pNode)->ulData;\
})*/
#define SLL_NODE_GET_DATA(pNode) ((SLL_NODE *)pNode)->ulData 

#define SLL_NODE_SET_DATA(pNode, pData) {\
    ((SLL_NODE *)pNode)->ulData = (ULONG)pData;\
}

void InsertNode(SLL *pList, SLL_NODE *pNode);
