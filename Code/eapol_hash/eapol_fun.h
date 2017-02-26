#include "type_def.h"
#include "public_def.h"

/********************宏定义start*********************/
#define MAC_SIZE 6
#define MAX_HASH_SIZE (48 * 1024)

#define GET_HASH_NODE_BY_INDEX(usIndex) &g_pEapHash[usIndex]
#define GET_HASH_HEAD_AND_TAIL() &g_eapHashHeadAndTail
#define GET_HASH_LINK_BY_INDEX(usIndex) &g_pusEapHaskKeyLinkHead[usIndex]

#define EAP_HASH_NODE_FREE 0
#define EAP_HASH_NODE_USED 1

/********************宏定义end*********************/

/********************结构体start*********************/

typedef struct
{
    USHORT usHead; /* 指向空闲链表的头，申请节点就由head指向的节点开始 */
    USHORT usTail; /* 指向空闲链表的尾，释放一个节点，尾巴就指向释放的那个节点，并且把被释放的节点挂在之前的尾巴上 */
} EAP_HASH_HEAD_TAIL;

typedef struct
{
    UCHAR szMac[MAC_SIZE];
    USHORT usNext; /* 值为被指向的全局数组的元素的index，根据index可以找到元素 */
    USHORT usPre;
    UCHAR ucUsedFlag;
} EAP_HASH_NODE;
/********************结构体end*********************/

/********************内部函数接口start****************/
extern VOID InitEapHashTable(VOID);
extern ULONG InsertEapHashNode(UCHAR *szMac, USHORT *pusNode);
extern ULONG DeleteEapHashNode(USHORT usNode);



/*******************内部函数接口end******************/