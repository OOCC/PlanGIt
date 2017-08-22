#include "BinarySearch.h"

int main()
{
    int a[1] = {1};
    int b[10] = {1,2,3,5,6,7,8,9,10};


    binarySearch(1, a, 1);
    binarySearch(0, a, 1);
    binarySearch(11, b, 10);
    binarySearch(3, b, 10);
    
}