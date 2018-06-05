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
UINT16 g_add_num = 0;
UINT16 g_del_num = 0;

UINT16 get_free_ptr()
{
	int ix = 0;
	for (; ix < MAX_PTR_NUM; ix++) {
		if (NULL == g_ptr[ix]) {
			g_add_num++;
			return ix;
		}

	}
	return 0xffff;
}

UINT16 get_used_ptr()
{
	int ix = 0;
	for (; ix < MAX_PTR_NUM; ix++) {
		if (NULL != g_ptr[ix]) {
			g_del_num++;
			return ix;
		}
	}
	return 0xffff;
}


void free_all_mem()
{
	int ix = 0;
	for (; ix < MAX_PTR_NUM; ix++) {
		if (NULL != g_ptr[ix]) {
			free_x(g_ptr[ix]);
			g_ptr[ix] = NULL;
		}
	}
	return;
}

int test()
{
	int ret = 0;
	int r = 0;
	int *ptr = NULL;
	UINT16 index = 0;
	int randd = 0;

	int a = calc_mem_level(1024);
	a = calc_mem_level(1023);
	a = calc_mem_level(1);
	a = calc_mem_level(1024+100);
	a = calc_mem_level(1024 + 1024);
	a = calc_mem_level(1024*3 -1);
	a = calc_mem_level(1024*3);





	while (1) {
		randd = randd++ % 7;
		srand((int)clock());
		r = ( ((UINT16)rand())/randd) ;
		if (r % 2) {
			ptr = malloc_x(r);
			if (NULL == ptr) {
				free_all_mem();
				continue;
			}
			/* save ptr to array */
			index = get_free_ptr();
			if (0xffff == index)
				return index;
			g_ptr[index] = ptr;
		}
		else {
			index = get_used_ptr();
			if (0xffff == index)
				continue;
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
