


void shell_sort(int arr[], int len) {
	int gap, i, j;
	int temp;
	for (gap = len >> 1; gap > 0; gap >>= 1)
		for (i = gap; i < len; i++) {
			temp = arr[i];
			for (j = i - gap; j >= 0 && arr[j] > temp; j -= gap)
				arr[j + gap] = arr[j];
			arr[j + gap] = temp;
		}
}