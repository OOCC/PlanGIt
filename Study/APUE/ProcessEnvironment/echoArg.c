#include <stdio.h>


/* shell run by this way */
int main(int argc, char *argv[])
{
    int i = 0;
    
    for (; i < argc; i++)
    {
        printf("%s\n",argv[i]);
    }
    return 0;
}
