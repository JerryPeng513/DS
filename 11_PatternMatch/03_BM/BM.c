/* BM.c */
#include <stdio.h>
#include <stdlib.h>

static void buildBC(char *pattern, int *bc, int lenP)
{
	memset(bc, -1, sizeof(int) * 256);
	int i = 0;
	for (; i < lenP; i ++)
	{
		bc[pattern[i]] = i;
	}
}

static int min(int a, int b)
{
	return (a < b ? a : b);
}

static void buildSS(char *pattern, int *ss, int lenP)
{
	ss[lenp - 1] = lenP;
	int lo = lenP - 1, hi = lenP - 1;
	int j = lo - 1;
	for (; j >= 0; j --)
	{
		if ((lo < j) && (ss[lenP - hi + j - 1] <= j - lo))
		{
			ss[j] = ss[lenP - hi + j - 1];
		}
		else
		{
			hi = j;
			lo = min(lo, hi);
			while ((0 <= lo) && (pattern[lo] ==pattern[lenP - hi + lo - 1]))
			{
				lo --;
			}
			ss[j] = hi - lo;
		}
	}
}

static void buildGS(char *pattern, int *gs, int lenP)
{
	int *ss = (int *)malloc(sizeof(int) * lenP);
	buildSS(pattern, ss, lenP);
	int j = 0;
	for (; j < lenP; j ++)
	{
		gs[j] = m;
	}
	
	
	free(ss);
}

static int max(int a, int b)
{
	return (a > b ? a : b);
}

static int match(char *pattern, int lenP, char *text, int lenT)
{
	int *bc = (int *)malloc(sizeof(int) * 256);	//bc表与字符表等长
	buildBC(pattern, bc, lenP);
	int *gs = (int *)malloc(sizeof(int) * lenP);
	buildGS(pattern, gs, lenP);
	int i = 0;
	while (i + lenP <= lenT)
	{
		int j = lenP - 1;
		while (pattern[j] == text[i + j])
		{
			if (0 > --j)
			{
				break;
			}
		}
		if (0 > j) //完全匹配
		{
			break;
		}
		else
		{
			i += max(gs[j], j - bc[text[i + j]]);
		}
	}
	free(bc);
	free(gs);
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