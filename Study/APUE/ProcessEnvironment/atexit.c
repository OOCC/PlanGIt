#include <stdio.h>
#include <stdlib.h>

void func1()
{
    printf("1\n");
}

void func2()
{
    printf("2\n");
}

int main()
{
    printf("main0\n");

    atexit(func1);
    atexit(func2);

    printf("main\n");

    return 0;
}