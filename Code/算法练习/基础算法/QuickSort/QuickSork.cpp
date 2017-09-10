void swap(int *x, int *y) {
	int t = *x;
	*x = *y;
	*y = t;
}
void quick_sort_recursive(int arr[], int start, int end) {
	if (start >= end)
		return;//這是為了防止宣告堆疊陣列時當機
	int mid = arr[end];
	int left = start, right = end - 1;
	while (left < right) {
		while (arr[left] < mid && left < right)
			left++;
		while (arr[right] >= mid && left < right)
			right--;
		swap(&arr[left], &arr[right]);
	}
	if (arr[left] >= arr[end])
		swap(&arr[left], &arr[end]);
	else
		left++;
    if (left) {
        quick_sort_recursive(arr, start, left - 1);
    }
    quick_sort_recursive(arr, left + 1, end);
}
void quick_sort(int arr[], int len) {
	quick_sort_recursive(arr, 0, len - 1);
}