/****************************************************************************
   文件名     : MemoryAllcation.h
   
   修改历史   :    
   1.  时间 :        2016-12-09  created by luoyefeisong
       内容 :        内存分配与管理相关定义
   2.  时间 :        
       内容 :
****************************************************************************/
/* memory total size */
#include "../include/linklist/linklist_pub.h"

#define MEM_SIZE   10240

/* means 2^n */
#define LL_ZERO    1
#define LL_ONE     2
#define LL_TWO     4
#define LL_THREE   8
#define LL_FOUR    16
#define LL_FIVE    32


#define MEM_MAX_ALLOCA 128

#define MEM_BLOCK_NUM  6

extern SLL_NODE  g_MemoryCtl[MEM_SIZE];
extern SLL g_pLLMemList[MEM_BLOCK_NUM];

VOID MEM_ALLOCA_INIT(VOID);


 void *malloc_x(ULONG ulSize);
   ULONG free_x(VOID *p);


