#include <stdio.h>
#include <setjmp.h>


jmp_buf jmpbuffer;

/* need to control input in use longjmp function */
int func()
{
    int a = 0;
    scanf("%d",&a);
    if (0 == a)
        longjmp(jmpbuffer, 1);

    printf("OK \n");

    return 0;
}


int do_switch(char i)
{
    func();
    return 0;
}

int main(int argc, char* argv[])
{
    int i = 0;

    if (0 != setjmp(jmpbuffer))
    {
        printf("error \n");
    }

    for (i = 0; i < argc; i++)
        do_switch(*argv[i]);

    return 0;
}


