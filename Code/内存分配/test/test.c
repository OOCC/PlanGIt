#include <stdio.h>
#include "../src/memoryallocation.h"




int main()
{
    MEM_ALLOCA_INIT();

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


}
