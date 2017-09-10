
#include <cstdlib>

void insertion_sort(int arr[], int len)  
{
   int i, j;
   int temp;
   for (i = 1; i < len; i++) 
   {
       temp = arr[i]; 
//與已排序的數逐一比較，大於temp時，該數向後移
       j = i - 1;  

        for (; j >= 0 && arr[j] > temp; j--) 
        {
            arr[j + 1] = arr[j];
        }
        arr[j + 1] = temp; 

   }
}