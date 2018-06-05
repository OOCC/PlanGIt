
#define MAX_LEVEL 10
#define MEM_SIZE 1024 * 1024
#define MEM_UNIT_SIZE 1  /* 单位都是KB， 1KB = 1024 BYTE */

extern char *g_MemoryData;
extern unsigned int  *g_bmp[MAX_LEVEL + 1];
extern SLL   g_pLLMemList[MAX_LEVEL + 1];

#define BMP_OFFSET(pnode, level)     ( ((char *)pnode - g_MemoryData) / 1024 / (1 << level))
VOID MEM_ALLOC_INIT(VOID);

void *malloc_x(ULONG ulSize);
ULONG free_x(VOID *p);

