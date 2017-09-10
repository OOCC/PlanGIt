
int swap(int arr[], int i, int j)
{
    int tmp;

    if (0 == arr)
        return -1;

    tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
    return 0;
}

int selectionSort(int arr[], int l, int r)
{
    int i, j, min;

    if (0 == arr)
        return -1;
    
	for (i = l; i < r; i++)
	{
		min = i;
		for (j = i + 1; j <= r; j++)
			if (arr[j] < arr[min])
				min = j;
		swap(arr, min, i);
	}
    return 0;
}