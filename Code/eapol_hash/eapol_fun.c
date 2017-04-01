#include "eapol_fun.h"
#include "type_def.h"
#include "public_def.h"
#include "C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.10150.0\\ucrt\\stdlib.h"
#include "C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.10150.0\\ucrt\\string.h"

/********************全局变量start*********************/
EAP_HASH_HEAD_TAIL g_eapHashHeadAndTail = {0};
EAP_HASH_NODE *g_pEapHash;
USHORT *g_pusEapHaskKeyLinkHead; // 以key为粒度的链表头,也就是空闲链表的头

/********************全局变量end*********************/

VOID InitEapHashTable(VOID)
{
    UINT32 i = 0;

    g_pEapHash = (EAP_HASH_NODE *)malloc(sizeof(EAP_HASH_NODE) * MAX_HASH_SIZE);
    memset(g_pEapHash, 0, sizeof(EAP_HASH_NODE) * MAX_HASH_SIZE);

    g_pusEapHaskKeyLinkHead = (USHORT *)malloc(sizeof(USHORT) * MAX_HASH_SIZE);
    memset(g_pusEapHaskKeyLinkHead, 0xff, sizeof(USHORT) * MAX_HASH_SIZE);

    for (i = 0; i < MAX_HASH_SIZE; i++)
    {
        g_pEapHash[i].usNext = i + 1;
        g_pEapHash[i].usPre = i - 1;
    }

    /* 最前面一个和最后面一个单独处理 */
    g_pEapHash[0].usPre = NULL_USHORT;
    g_pEapHash[MAX_HASH_SIZE - 1].usNext = NULL_USHORT;

    /* 初始化头尾指针 */
    g_eapHashHeadAndTail.usHead = 0;
    g_eapHashHeadAndTail.usTail = MAX_HASH_SIZE - 1;
    return;
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
ULONG MallocEapNode(USHORT *pusNode)
{
    EAP_HASH_NODE *pHash = NULL;
    EAP_HASH_HEAD_TAIL *pHead = GET_HASH_HEAD_AND_TAIL();
	EAP_HASH_NODE *pHashNext = NULL;

    /* 链表已经满了 */
    if (NULL_USHORT == pHead->usHead)
    {
        return VOS_ERR;
    }

    *pusNode = pHead->usHead; /* 头节点就是要申请的节点 */

    pHash = GET_HASH_NODE_BY_INDEX(*pusNode);
    pHash->usPre = NULL_USHORT; /* 将申请到的节点断掉，表示已经被占用了 */

    pHead->usHead = pHash->usNext; /* 头就移动到下一个节点的位置了 */

    if (NULL_USHORT == pHash->usNext)
    {
        pHead->usTail = NULL_USHORT; /* 因为链表已经分配完了，头倒是知道，但是尾还不知道，要单独赋值 */
    }
	else /* 有next就将next的前指针置为全f */
	{
		pHashNext = GET_HASH_NODE_BY_INDEX(pHash->usNext);
		pHashNext->usPre = NULL_USHORT;
	}

    pHash->ucUsedFlag = EAP_HASH_NODE_USED;
    return VOS_OK;
}

/* 申请节点，并且挂载hash表 */
ULONG InsertEapHashNode(UCHAR *szMac, USHORT *pusNode)
{
    USHORT usKey = 0;
    USHORT *pLink = NULL;
    EAP_HASH_NODE *pHash = NULL;
    EAP_HASH_NODE *pHashOld = NULL;


    if (NULL == szMac)
    {
        return VOS_ERR;
    }

    if (VOS_OK != MallocEapNode(pusNode))
    {
        return VOS_ERR;
    }

    pHash = GET_HASH_NODE_BY_INDEX(*pusNode);

    usKey = GetHashKeyForMac(szMac);
    /* 得到对应key的链表头节点 */
    pLink = GET_HASH_LINK_BY_INDEX(usKey);

    /* 头插法，第一个使用节点的usPre都是指向全f */
    pHash->usPre = NULL_USHORT;
    if (NULL_USHORT == *pLink)
    {
        pHash->usNext = NULL_USHORT;
    }
    else
    {
        pHashOld = GET_HASH_NODE_BY_INDEX(*pLink);
        pHashOld->usPre = *pusNode;

        pHash->usNext = *pLink;
    }
    *pLink = *pusNode;

    memcpy(pHash->szMac, szMac, MAC_SIZE);
    return VOS_OK;
}

/* 释放一个占用的节点 */
VOID FreeEapNode(USHORT usNode)
{
    EAP_HASH_NODE *pHash = NULL;
    EAP_HASH_NODE *pHashPre = NULL;
    EAP_HASH_HEAD_TAIL *pHead = GET_HASH_HEAD_AND_TAIL();

    pHash = GET_HASH_NODE_BY_INDEX(usNode);

    pHash->usPre = pHead->usTail; /* 将释放的节点接在尾巴后 */
    pHash->usNext = NULL_USHORT;
    pHead->usTail = usNode; /* 将尾巴定位为释放的节点，释放的节点成了新的尾巴 */

    if (NULL_USHORT == pHead->usHead) /* 如果链表已经是满的了，就没有上一个节点。不需要找到上一个节点，将上一个节点的next指向被释放的节点 */
    {
        pHead->usHead = usNode;
    }
    else /* 如果链表没有满，那么需要找到被释放节点的上一个节点，将上一个节点的next指向现在被释放的节点 */
    {
        pHashPre = GET_HASH_NODE_BY_INDEX(pHash->usPre);
        pHashPre->usNext = usNode;
    }
    pHash->ucUsedFlag = EAP_HASH_NODE_FREE;
    return;
}

/* 删除一个节点 */
ULONG DeleteEapHashNode(USHORT usNode)
{
    EAP_HASH_NODE *pHash = NULL;
    EAP_HASH_NODE *pHashNext = NULL;
    EAP_HASH_NODE *pHashPre = NULL;
    USHORT usKey = NULL_USHORT;
    USHORT *pLink = NULL;

    if (usNode >= MAX_HASH_SIZE)
    {
        return VOS_ERR;
    }

    pHash = GET_HASH_NODE_BY_INDEX(usNode);

    if (pHash->ucUsedFlag != EAP_HASH_NODE_USED)
    {
        return VOS_ERR;
    }

    /* 如果删除的是使用链表中的第一个节点 */
    if (NULL_USHORT == pHash->usPre)
    {
        usKey = GetHashKeyForMac(pHash->szMac);
        pLink = GET_HASH_LINK_BY_INDEX(usKey);

        if (*pLink != usNode)
        {
            return VOS_ERR;
        }
        *pLink = pHash->usNext;
		if (NULL_USHORT != pHash->usNext)
		{
			pHashNext = GET_HASH_NODE_BY_INDEX(pHash->usNext);
			pHashNext->usPre = NULL_USHORT;
		}

    }
    else //不是使用链表中的第一个节点
    {
        pHashPre = GET_HASH_NODE_BY_INDEX(pHash->usPre);
        pHashPre->usNext = pHash->usNext;

        /* 也不是使用链表中的最后一个节点 */
        if (NULL_USHORT != pHash->usNext)
        {
            pHashNext = GET_HASH_NODE_BY_INDEX(pHash->usNext);
            pHashNext->usPre = pHash->usPre;
        }
    }

    FreeEapNode(usNode);

    memset(pHash->szMac, 0x0, MAC_SIZE);
    return VOS_OK;
}