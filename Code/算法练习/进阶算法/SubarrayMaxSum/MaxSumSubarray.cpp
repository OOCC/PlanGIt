#include<iostream>
#include <vector>

using std::vector;

/* 求子数组最大和，思路是如果数组内有正有负，最大子数组一定是从非负元素开始。 */
int maxSumSubarray(int a[])
{
	if (vec.empty())
		return 0xffffffff;

	int cur = 0;
	int max = 0;

	vector<int>::iterator iter = (*begin).begin();

	auto auto_iter = begin;
	
	cur = max = *iter;

	for (++iter; iter != (*end).end(); iter++)
	{
		cur += *iter;

		if (cur > max)
			max = cur;

		if (cur < 0)
			cur = 0;
	}

	return max;
}


int test(vector<vector<int>> &vec, vector<vector<int>>::iterator begin, vector<vector<int>>::iterator end) {
	
	std::cout << "++++++++++++++++++++++" << std::endl;
	return 0;
}