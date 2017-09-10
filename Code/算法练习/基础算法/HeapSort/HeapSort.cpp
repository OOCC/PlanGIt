#include <cstdlib>

/* 递归堆排序，每次找一个二叉树，把所有二叉树排序则排序完成 */
int maxHeapify(int arr[], int index, int heapSize)
{
    int iMax = index,
        iLeft = 2 * index + 1,
        iRight = 2 * (index + 1);
    int tmp = 0;

    if (NULL == arr) {
        return -1;
    }
    

    if (iLeft < heapSize && arr[index] < arr[iLeft]) {
        iMax = iLeft;
    }

    if (iRight < heapSize && arr[iMax] < arr[iRight]) {
        iMax = iRight;
    }

    if (iMax != index) {
        tmp = arr[iMax];
        arr[iMax] = arr[index];
        arr[index] = tmp;
        
		maxHeapify(arr, iMax, heapSize);
    }   

    return 0;
}

/* 将一个数组改造成最大堆，但是同一层的顺序还未排好 */
int buildMaxHeap(int arr[], int heapSize)
{   
    int iParent = (heapSize/2)-1;

    for (int i = iParent; i >= 0; i--) {
		maxHeapify(arr, i, heapSize);
    }
    return 0;
}


int heapSort(int arr[], int heapSize)
{
    buildMaxHeap(arr, heapSize);
    int tmp = 0;

    for (int i = heapSize - 1; i > 0; i--) {
        tmp = arr[0];
        arr[0] = arr[i];
        arr[i] = tmp;

		maxHeapify(arr, 0, i);
    }
	return 0;
}

