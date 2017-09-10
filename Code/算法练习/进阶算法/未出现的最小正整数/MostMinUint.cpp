

/*
题目：
给定一个无序整形数组arr，找到数组中未出现的最小正整数。
【举例】
arr = [-1,2,3,4] return 1
arr = [1,2,3,4] return 5
【要求】
时间复杂度O(N), 额外空间复杂度O(1)
*/

/*
解题思路：
类似于动态规划的思路

1. l 遍历到目前为止的最小正整数减一
2. r 目前为止所有数可能的最小正整数

*/


void swp(int iArr[], int i, int j, int len)
{
    int tmp = 0;

    if (i >= len || j >= len)
    {
        return;
    }
    tmp = iArr[i];
    iArr[i] = iArr[j];
    iArr[j] = tmp;
}

unsigned int mostMinUint(int iArr[], int len)
{
    int l = 0;
    int r = len;

    while (l < r)
    {
        if ( iArr[l] == l + 1)
        {
            l++;
        }
        /* 1. 如果当前数大于等于理想值r，说明r的范围需要减小了。 
           2. 如果当前数小于等于当前位置的理想值，也就是比如下标1位置的理想值是2，若干<=1, 说明当前位置的值已经不理想。
           3.  */
        else if (iArr[l] > r || iArr[l] <= l )
        {
            iArr[l] = iArr[--r];
        }
        else
        {
            swp(iArr, l, iArr[l]-1, len);
        }
    }
    return l + 1;
}


