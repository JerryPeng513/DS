/* KMP.c */
#include <stdio.h>
#include <stdlib.h>

void buildNext(char *pattern, int *next, int len)
{
	next[0] = -1;
	int i = 0, t = -1;
	while (i < len - 1)
	{
		(0 > t || pattern[i] == pattern[t]) ? (next[++ i] = ++ t) : (t = next[t]);
	}
}

void buildNextImprove(char *pattern, int *next, int len)
{
	next[0] = -1;
	int i = 0, t = -1;
	while (i < len - 1)
	{
		if (0 > t || pattern[i] == pattern[t]) //匹配
		{
			i ++;
			t ++;
			//引入“负面”信息
			if (pattern[i] != pattern[t])
			{
				next[i] = t;
			}
			else
			{
				next[i] = next[t];
			}
		}
		else //失配
		{
			t = next[t];
		}
	}
}

static int match(char *pattern, int lenP, char *text, int lenT)
{
	int *next = (int *)malloc(sizeof(int) * lenP);	
	buildNextImprove(pattern, next, lenP);
	int i = 0, j = 0;
	while (i < lenT && j < lenP)
	{
		if (0 > j || text[i] == pattern[j])
		{
			i ++; 
			j ++;
		}
		else
		{
			//失配在第0个位置时，next[0]=-1
			j = next[j];
		}
	}
	free(next);
	return i - j;
}

int main()
{
	int n, m;
	scanf("%d", &n);
	char *A = (char *)malloc(n + 1);
	scanf("%s", A);
	A[n] = '\0';
	scanf("%d", &m);
	char *B = (char *)malloc(m + 1);
	scanf("%s", B);
	B[m] = '\0';
	int res = match(A, n, B, m);
	if (res <= m - n)
	{
		printf("match success\n");
	}
	else
	{
		printf("match fail\n");
	}
	free(A);
	free(B);
	return 0;
}