#include <stdio.h>
#include "public_def.h"
#include "type_def.h"
#include "linklist_pub.h"
#include "buddy_algorithm.h"
#include "bitmap.h"



int main()
{
	//MEM_ALLOC_INIT();


	UINT32 *bitmap[10] = { 0 };
	INT32 ix = 0;
	ULONG request_size = 1024 * 1024 / 32;
	bmp_t *parent = NULL;
	char key;

	for (ix = 0; ix < 10; ix++) {
		bmp_create(&bitmap[ix], request_size);
		request_size /= 2;
	}


	for (ix = 0; ix < 10; ix++) {
		bmp_add(bitmap[ix], ix);
	}


	scanf_s("%c", &key);

	for (ix = 0; ix < 10; ix++) {
		parent = container_of(bitmap[ix], bmp_t, pbits);
		bmp_delete(parent->pbits);
	}
	return 0;

/*

    int *p1 = NULL;
    int *p2 = NULL;
    int *p3 = NULL;
    int *p4 = NULL;
    int a = 0;
    SLL_NODE *p = NULL;

    p1 = malloc_x(31);
    p = (SLL_NODE *)p1;
    p2 = malloc_x(3);
    p = (SLL_NODE*)p2;



    free_x(p1);
    free_x(p2);

    p3 = malloc_x(21);
    p = (SLL_NODE *)p3;
    p4 = malloc_x(10);
    p = (SLL_NODE*)p4;



    free_x(p3);
    free_x(p4);

    a = 1;

	*/
}
