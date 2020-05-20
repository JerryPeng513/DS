/* BTreeTest.c */
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include "BTree.h"

static int IntCmp(const void *keyAddr, const void *dataAddr)
{
	int *p1 = (int *)keyAddr;
	int *p2 = (int *)dataAddr;
	return (*p1 - *p2);
}

static void IntTraverse(void *keyAddr, void *outData)
{
	int *p = (int *)keyAddr;
	printf("%d\n", *p);
}

static int StringCmp(const void *keyAddr, const void *dataAddr)
{
	char *p1 = *(char **)keyAddr;
	char *p2 = *(char **)dataAddr;
	return strcmp(p1, p2);
}

static void StringTraverse(void *keyAddr, void *outData)
{
	char *p = *(char **)keyAddr;
	printf("%s\n", p);
}

static void StringFree(void *keyAddr)
{
	char *p = *(char **)keyAddr;
	free(p);
}

int main()
{
	BTREE intBTree;
	BTreeNew(&intBTree, 3, sizeof(int), IntCmp, NULL);
	printf("order of intBTree = %d\n", BTreeOrder(&intBTree));
	int i = 0;
	for (; i < 10; i++)
	{
		BTreeInsert(&intBTree, &i);
	}
	if (!BTreeEmpty(&intBTree))
	{
		printf("size of intBtree = %d\n", BTreeSize(&intBTree));
		BTreeTravLevel(&intBTree, IntTraverse, NULL);
	}
	int intRemove = 1;
	if (0 == BTreeRemove(&intBTree, &intRemove))
	{
		printf("intBTree remove key %d success\n", intRemove);
	}
	else
	{
		printf("intBTree remove key %d fail\n", intRemove);
	}
	if (0 == BTreeRemove(&intBTree, &intRemove))
	{
		printf("intBTree remove key %d success\n", intRemove);
	}
	else
	{
		printf("intBTree remove key %d fail\n", intRemove);
	}
	if (!BTreeEmpty(&intBTree))
	{
		printf("size of intBtree = %d\n", BTreeSize(&intBTree));
		BTreeTravLevel(&intBTree, IntTraverse, NULL);
	}
	int intSearch = 2;
	if (NULL != BTreeSearch(&intBTree, &intSearch))
	{
		printf("data %d is in intBTree\n", intSearch);
	}
	else
	{
		printf("data %d is not in intBTree\n", intSearch);
	}
	intSearch = 11;
	if (NULL != BTreeSearch(&intBTree, &intSearch))
	{
		printf("data %d is in intBTree\n", intSearch);
	}
	else
	{
		printf("data %d is not in intBTree\n", intSearch);
	}
	BTreeDispose(&intBTree);
	printf("\n\n");
	BTREE stringBTree;
	BTreeNew(&stringBTree, 3, sizeof(char *), StringCmp, StringFree);
	char *name1 = strdup("pc");
	char *name2 = strdup("pcwl513");
	char *name3 = strdup("pcpc");
	char *name4 = strdup("jerry");
	char *name5 = strdup("jerry.peng");
	char *name6 = strdup("yanglupu");
	char *name7 = strdup("zhanglei");
	char *name8 = strdup("lishanke");
	BTreeInsert(&stringBTree, &name1);
	BTreeInsert(&stringBTree, &name2);
	BTreeInsert(&stringBTree, &name3);
	BTreeInsert(&stringBTree, &name4);
	BTreeInsert(&stringBTree, &name5);
	BTreeInsert(&stringBTree, &name6);
	BTreeInsert(&stringBTree, &name7);
	BTreeInsert(&stringBTree, &name8);
	if (!BTreeEmpty(&stringBTree))
	{
		printf("size of stringBTree = %d\n", BTreeSize(&stringBTree));
		BTreeTravLevel(&stringBTree, StringTraverse, NULL);
	}
	char *strRemove = "pcpc";
	if (0 == BTreeRemove(&stringBTree, &strRemove))
	{
		printf("stringBTree remove key %s success\n", strRemove);
	}
	else
	{
		printf("stringBTree remove key %s fail\n", strRemove);
	}
	if (0 == BTreeRemove(&stringBTree, &strRemove))
	{
		printf("stringBTree remove key %s success\n", strRemove);
	}
	else
	{
		printf("stringBTree remove key %s fail\n", strRemove);
	}
	if (!BTreeEmpty(&stringBTree))
	{
		printf("size of stringBTree = %d\n", BTreeSize(&stringBTree));
		BTreeTravLevel(&stringBTree, StringTraverse, NULL);
	}
	char *strSearch = "yanglupu";
	if (NULL != BTreeSearch(&stringBTree, &strSearch))
	{
		printf("data %s is in stringBTree\n", strSearch);
	}
	else
	{
		printf("data %s is not in stringBTree\n", strSearch);
	}
	strSearch = "123";
	if (NULL != BTreeSearch(&stringBTree, &strSearch))
	{
		printf("data %s is in stringBTree\n", strSearch);
	}
	else
	{
		printf("data %s is not in stringBTree\n", strSearch);
	}	
	BTreeDispose(&stringBTree);
	return 0;
}