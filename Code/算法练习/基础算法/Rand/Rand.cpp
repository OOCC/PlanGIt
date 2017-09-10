#include <ctime>
#include <cstdlib>

int CreatRandArr(int arr[], int arrSize)
{
	int i;
	srand((unsigned)time(NULL));

    for(i = 0; i < arrSize; i++){
		arr[i] = rand() % (arrSize + 1);
    }
	return 0;
}