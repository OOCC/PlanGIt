#include <stdio.h>
#include "../src/memoryallocation.h"




int main()
{
    MEM_ALLOCA_INIT();

    int *p1 = NULL;
    int *p2 = NULL;
	SLL_NODE *p = NULL;

    p1 = malloc_x(31);
	p = (SLL_NODE *)p1;

    p2 = malloc_x(3);
	p = (SLL_NODE *)p2;



    free_x(p1);
    free_x(p2);
    
    
}
