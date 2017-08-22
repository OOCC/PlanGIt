
int binarySearch(int data, int a[], int len)
{
    if (0 == a)
        return -1;
    
    int left = 0;
    int right = len - 1;
    int mid = 0;

    while (left <= right)
    {
        mid = (left + right) / 2;
        
        if (data > a[mid])
            left = mid + 1;
        else if (data < a[mid])
            right = mid - 1;
        else
            return mid;
    }

    return -1;
}