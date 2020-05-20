/* KSelection_QS.c */
#include <stdio.h>
#include <stdlib.h>

//选取一个序列中第k大的元素
//选中位数是k选取(一组有序的序列中选择第k个元素)算法的特例
//且选取中位数是k选取算法中时间最长的
//基于快速划分的K选取算法
int kSelection_QS(int *seq, int n, int k)
{
	int lo = 0, hi = n - 1;
	int tar = k - 1;
	for (; lo < hi;)
	{
		int i = lo, j = hi;
		int pivot = seq[lo];
		while (i < j)
		{
			while ((i < j) && (pivot <= seq[j]))
			{
				j --;
			}
			seq[i] = seq[j];
			while ((i < j) && (pivot >= seq[i]))
			{
				i ++;
			}
			seq[j] = seq[i];
		}
		seq[i] = pivot;
		if (tar == i)
		{
			break;
		}
		else if (tar < i)
		{
			hi = i - 1;
		}
		else
		{
			lo = i + 1;
		}
	}
	return seq[tar];
}

int main()
{
	int n, k;
	scanf("%d%d", &n, &k);
	int *a = (int *)malloc(sizeof(int) * n);	
	int i = 0;
	for (; i < n; i ++)
	{
		scanf("%d", &a[i]);	
	}
	printf("%d\n", kSelection_QS(a, n, k));
	free(a);
	return 0;
}