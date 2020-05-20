/* BruteForce.c */
#include <stdio.h>
#include <stdlib.h>

static int matchA(char *pattern, int lenP, char *text, int lenT)
{
	int i = 0, j = 0;
	while (j < lenP && i < lenT)
	{
		if (pattern[j] == text[i])
		{
			i ++;
			j ++;
		}
		else
		{
			i -= j - 1;
			j = 0;
		}
	}
	return i - j;
}

static int matchB(char *pattern, int lenP, char *text, int lenT)
{
	int i = 0, j;
	for (; i < lenT - lenP + 1; i ++)
	{
		for (j = 0; j < lenP; j ++)
		{
			if (text[i + j] != pattern[j])
			{
				break;
			}
		}
		if (lenP <= j) //完全匹配
		{
			break;
		}
	}
	return i;
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
	int res = matchA(A, n, B, m);
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