#include <stdio.h>
#include "public_def.h"
#include "type_def.h"
#include "linklist_pub.h"
#include "buddy_algorithm.h"
#include "bitmap.h"
#include <stdlib.h>
#include <time.h>


#define MAX_PTR_NUM 65535
int *g_ptr[MAX_PTR_NUM] = { 0 };


UINT16 get_free_ptr()
{
	int ix = 0;
	for (; ix < MAX_PTR_NUM; ix++) {
		if (NULL == g_ptr[ix]) {
			return ix;
		}

	}
	return NULL;
}

UINT16 get_used_ptr()
{
	int ix = 0;
	for (; ix < MAX_PTR_NUM; ix++) {
		if (NULL != g_ptr[ix])
			return ix;
	}
	return NULL;
}


int test()
{
	int ret = 0;
	int r = 0;
	int *ptr = NULL;
	UINT16 index = 0;

	while (1) {
		srand((int)time(NULL));
		r = (UINT16)rand();
		if (r % 2) {
			ptr = malloc_x(r);
			/* save ptr to array */
			index = get_free_ptr();
			g_ptr[index] = ptr;
		}
		else {
			index = get_used_ptr();
			free_x(g_ptr[index]);
			g_ptr[index] = NULL;
		}
	}
	
	return ret;
}



int main()
{
	MEM_ALLOC_INIT();

	int *p1 = NULL;
    int *p2 = NULL;
    int *p3 = NULL;
    int *p4 = NULL;
    int a = 0;
    SLL_NODE *p = NULL;

    p1 = malloc_x(31);
    p = (SLL_NODE *)p1;
    free_x(p1);


    p3 = malloc_x(21);
    p = (SLL_NODE *)p3;
    p4 = malloc_x(10000000);
    p = (SLL_NODE*)p4;
	free_x(p3);
    free_x(p4);

	test();

	return 0;

}
