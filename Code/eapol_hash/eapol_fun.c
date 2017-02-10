#include <stdio.h>
#include "type_def.h"
#include "public_def.h"

/********************宏定义*********************/
#define MAC_SIZE  6
#define MAX_HASH_SIZE  (48*1024)

#define GET_HASH_NODE_BY_INDEX(usIndex)  &g_pEapHash[usIndex]

/********************宏定义*********************/



/********************结构体*********************/

typedef struct
{
    USHORT usHead; /* 指向空闲链表的头，申请节点就由head指向的节点开始 */
    USHORT usTail; /* 指向空闲链表的尾，释放一个节点，尾巴就指向释放的那个节点，并且把被释放的节点挂在之前的尾巴上 */
}EAP_HASH_HEAD_TAIL;

typedef struct{
    UCHAR szMac[MAC_SIZE]
    USHORT usNext;  /* 值为被指向的全局数组的元素的index，根据index可以找到元素 */
    USHORT usPre;   
}EAP_HASH_NODE;
/********************结构体*********************/

/********************全局变量*********************/
EAP_HASH_HEAD_TAIL g_eapHashHeadAndTail = {0};
EAP_HASH_NODE *g_pEapHash;
USHORT *g_pusEapHaskKeyLinkHead; /* 以key为粒度的链表头 */

/********************全局变量*********************/


VOID InitEapHashTable(VOID)
{
    UINT32 i = 0;

    g_pEapHash = (EAP_HASH_NODE *)malloc(sizeof(EAP_HASH_NODE)*MAX_HASH_SIZE);
    memset(g_pEapHash,0,sizeof(EAP_HASH_NODE)*MAX_HASH_SIZE);

    g_pusEapHaskKeyLinkHead = (USHORT*)malloc(sizeof(USHORT)*MAX_HASH_SIZE)
    memset(g_pusEapHaskKeyLinkHead,0xf,sizeof(USHORT)*MAX_HASH_SIZE)

    for (i = 0; i < MAX_HASH_SIZE; i++)
    {
        g_pEapHash[i].usNext = i + 1;
        g_pEapHash[i].usPre = i - 1;
    }

    /* 最前面一个和最后面一个单独处理 */
    g_pEapHash[0].usPre = 0xffff;
    g_pEapHash[MAX_HASH_SIZE-1].usNext = 0xffff;

    /* 初始化头尾指针 */
    g_eapHashHeadAndTail.usHead = 0;
    g_eapHashHeadAndTail.usTail = MAX_HASH_SIZE-1;
}

USHORT GetHashKeyForMac(UCHAR *szMac)
{
    USHORT i = 0;
    USHORT j = 0;
    for (i = 0; i < MAC_SIZE; i++)
    {
        j *= 73;
        j ^= szMac[i];
    }
    return (j % MAX_HASH_SIZE);
}

/* 申请一个空闲节点 */
ULONG MallocEapNode(USHORT *pNode)
{
    EAP_HASH_NODE *pHash = NULL;

    /* 链表已经满了 */
    if (0xffff == g_eapHashHeadAndTail.usHead )
    {
        return VOS_ERR;
    }


    *pNode =  g_eapHashHeadAndTail.usHead;  /* 头节点就是要申请的节点 */
    
    pHash = GET_HASH_NODE_BY_INDEX(*pNode);
    pHash->usPre = 0xffff;  /* 将申请到的节点断掉，表示已经被占用了 */ 
    
    g_eapHashHeadAndTail.usHead = pHash->usNext; /* 头就移动到下一个节点的位置了 */

    if (0xffff == pHash->usNext)
    {
        g_eapHashHeadAndTail.usTail = 0xffff; /* 因为链表已经分配完了，头倒是知道，但是尾还不知道，要单独赋值 */
    }
    

}

/* 申请节点，并且挂载hash表 */
ULONG InsertEapHashNode(UCHAR *szMac, USHORT *pNode)
{
    USHORT usKey = 0;

    if (NULL == szMac || NULL == pNode)
    {
        return VOS_ERR;
    }

    if ( VOS_OK != MallocEapNode(pNode) )
    {
        return VOS_ERR;
    }
    
    usKey = GetHashKeyForMac(szMac);
    

}

/* 释放一个占用的节点 */
ULONG FreeEapNode(USHORT pNode)
{
    EAP_HASH_NODE pHash = NULL;
    EAP_HASH_NODE pHashPre = NULL;

    if (pNode >= MAX_HASH_SIZE) /* 这里不判断可能会出现什么异常? */
    {
        return VOS_ERR;
    }

    pHash = GET_HASH_NODE_BY_INDEX(pNode);

    pHash->usPre = g_eapHashHeadAndTail.usTail; /* 将释放的节点接在尾巴后 */
    pHash->usNext = 0xffff;     
    g_eapHashHeadAndTail.usTail = pNode;  /* 将尾巴定位为释放的节点，释放的节点成了新的尾巴 */

    if (0xffff == g_eapHashHeadAndTail.usHead )    /* 如果链表已经是满的了，就没有上一个节点。不需要找到上一个节点，将上一个节点的next指向被释放的节点 */
    {
        g_eapHashHeadAndTail.usHead = pNode;
    }
    else           /* 如果链表没有满，那么需要找到被释放节点的上一个节点，将上一个节点的next指向现在被释放的节点 */
    {
        pHashPre = GET_HASH_NODE_BY_INDEX(pHash->usPre);
        pHashPre->usNext = pNode;
    }

}

ULONG DeleteEapHashNode(USHORT *pNode)
{

}