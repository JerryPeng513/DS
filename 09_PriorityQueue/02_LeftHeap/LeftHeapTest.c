/* LeftHeapTest.c */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include "LeftHeap.h"

static int StringCmp(const void *keyAddr, const void *elemAddr)
{
	char *p1 = *(char **)keyAddr;
	char *p2 = *(char **)elemAddr;
	return strcmp(p1, p2);
}
static void StringFree(void *elemAddr)
{
	free(*(char **)elemAddr);
}

int main()
{
	PQUEUE pq;
	PQueueNew(&pq, sizeof(char *), StringCmp, StringFree);
	char *name1 = strdup("pc");
	char *name2 = strdup("jerry");
	char *name3 = strdup("hada");
	char *name4 = strdup("sunanzhi");
	char *name5 = strdup("zhaoyun");
	char *name6 = strdup("xiejinying");
	char *name7 = strdup("yuzhiqiang");
	char *name8 = strdup("liyunlong");
	char *name9 = strdup("luyuebin");
	char *name10 = strdup("lihui");
	char *name11 = strdup("renwenjie");
	char *name12 = strdup("chenzhaojie");
	char *name13 = strdup("zhangyouhe");
	PQueueInsert(&pq, &name1);
	PQueueInsert(&pq, &name2);
	PQueueInsert(&pq, &name3);
	PQueueInsert(&pq, &name4);
	PQueueInsert(&pq, &name5);
	PQueueInsert(&pq, &name6);
	PQueueInsert(&pq, &name7);
	PQueueInsert(&pq, &name8);
	PQueueInsert(&pq, &name9);
	PQueueInsert(&pq, &name10);
	PQueueInsert(&pq, &name11);
	PQueueInsert(&pq, &name12);
	PQueueInsert(&pq, &name13);
	if (!PQueueEmpty(&pq))
	{
		printf("pq size is %d\n", PQueueSize(&pq));
		char *eMax;
		if (0 == PQueueGetMax(&pq, &eMax))
		{
			printf("pq max data is %s\n", eMax);
		}
	}
	while (!PQueueEmpty(&pq))
	{
		PQueueDeleteMax(&pq);
		printf("pq size is %d\n", PQueueSize(&pq));
		char *eMax;
		if (0 == PQueueGetMax(&pq, &eMax))
		{
			printf("pq max data is %s\n", eMax);
		}
	}
	PQueueDispose(&pq);
	return 0;
}