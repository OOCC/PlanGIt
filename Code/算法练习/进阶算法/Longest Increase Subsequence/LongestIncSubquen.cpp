
int g_b_len = 0;

int findPosition(int a[], int data)
{
    int left = 0;
    int right = g_b_len - 1;
	int mid = 0;

    while (left <= right)
    {
        if (data > a[right])
            return right;
        if (data < a[left])
            return left;
        
		mid = (left + right) / 2;

        if (data == a[mid])
            return -1;
        else if (data > a[mid])
            left = mid + 1;
        else
            right = mid - 1;
    }
    return -1;
}

int b[10];

int *longestIncSubquen(int a[], int len)
{
    if (0 == a)
        return 0;

    b[0] = a[0];
    int ral = 0;
    g_b_len = 1;

    for (int i = 1; i < len; i++)
    {
        ral = findPosition(b, a[i]);
        if (-1 != ral )
        {
			/* 返回的坐标是子序列的最后一个元素 */
			if (ral == g_b_len - 1)
			{
				if (a[i] > b[ral])
				{
					b[g_b_len] = a[i];
					g_b_len++;
				}
				else
				{
					b[g_b_len - 1] = a[i];
				}
			}
            else 
			{
				if (a[i] > b[ral])
				{
					b[++ral] = a[i];
				}
				else
				{
					b[ral] = a[i];
				}
            }
        }
    }

    return &b[0];
}