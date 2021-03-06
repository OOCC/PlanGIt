/****************************************************************************
   文件名     : linklist_pub.h
   
   修改记录   :    
   1.  时间 :        2016-02-28 created by luoyefeisong
       内容 ：       双向链表
       
   2.  时间 ：       2016-12-08 modify by xueyu 
       内容 ：       还是写成单向链表吧，链表尾就是指向NULL，这样好判断 

****************************************************************************/
//#include "/TEST_LinkList/"

#include <stdio.h>
#include <stdbool.h>

#include "public_def.h"
#include "type_def.h"
    
typedef struct node {
    struct node  *pNext;
    char *address;
	char used;
}SLL_NODE;


/* 链表头 */
typedef struct {
    SLL_NODE stHead;    /* 链表头 */   
    SLL_NODE *pstTail;  /* 一直等于最后一个节点的地址，方便使用尾插法插入数据 */
    ULONG    ulNodeNum; /* 节点个数 */
}SLL;  /* 头是结构体，采用尾插法 */

#if DESC(" function")

SLL_NODE *FindFreeNodebyLevel(int level);
SLL_NODE *FindNodeByList(SLL *pList, char *);
ULONG FindNodeByAddress(char *address, int *level, SLL_NODE **ppNode);
SLL_NODE *FindNode(ULONG level, char *address);
void InsertNode(SLL *pList, char *address, char used);
void DeleteNode(SLL *pList, char *address);
#endif



#define SLL_INIT(pList) {\
	((SLL *)pList)->stHead.pNext = ((SLL *)pList)->pstTail;\
    ((SLL *)pList)->pstTail = NULL;\
    ((SLL *)pList)->ulNodeNum = 0;\
}

/*#define SLL_COUNT(pList) {\
	((SLL *)pList)->ulNodeNum;\
}*/
#define SLL_COUNT(pList) ((SLL *)pList)->ulNodeNum

#define SLL_ADD(pList, address, used) {\
    InsertNode(pList, address, used);\
}

#define SLL_DEL(pList, pNode) {\
    DeleteNode(pList, pNode);\
} 


#define SLL_NODE_INIT(pNode) {\
    ((SLL_NODE *)pNode)->pNext = NULL;\
    ((SLL_NODE *)pNode)->ulBlockIndex = NULL_ULONG;\
    ((SLL_NODE *)pNode)->bFree = false;\
} 

/*#define SLL_NODE_GET_DATA(pNode) ({\
    ((SLL_NODE *)pNode)->ulData;\
})*/


