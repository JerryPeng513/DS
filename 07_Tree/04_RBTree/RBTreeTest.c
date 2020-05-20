/* RBTreeTest.c */
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include "RBTree.h"

static int IntCmp(const void *keyAddr, const void *dataAddr)
{
	int *p1 = (int *)keyAddr;
	int *p2 = (int *)dataAddr;
	return (*p1 - *p2);
}

static void IntTraverse(void *dataAddr)
{
	int *p = (int *)dataAddr;
	printf("%d\n", *p);
}

static int StringCmp(const void *keyAddr, const void *dataAddr)
{
	char *p1 = *(char **)keyAddr;
	char *p2 = *(char **)dataAddr;
	return strcmp(p1, p2);
}

static void StringTraverse(void *dataAddr)
{
	char *p = *(char **)dataAddr;
	printf("%s\n", p);
}

static void StringFree(void *dataAddr)
{
	char *p = *(char **)dataAddr;
	free(p);
}

int main()
{
	RBTREE intRBTree;
	RBTreeNew(&intRBTree, sizeof(int), IntCmp, NULL);
	int i = 0;
	for (; i < 10; i++)
	{
		RBTreeInsert(&intRBTree, &i);
	}
	if (!RBTreeEmpty(&intRBTree))
	{
		printf("intRBTree size is %d\n", RBTreeSize(&intRBTree));
		printf("intRBTree height is %d\n", RBTreeHeight(&intRBTree));
		RBTreeTravIn(&intRBTree, IntTraverse);
	}
	int intRemove = 1;
	if (0 == RBTreeRemove(&intRBTree, &intRemove))
	{
		printf("intRBTree remove key %d success\n", intRemove);
	}
	else
	{
		printf("intRBTree remove key %d fail\n", intRemove);
	}
	if (0 == RBTreeRemove(&intRBTree, &intRemove))
	{
		printf("intRBTree remove key %d success\n", intRemove);
	}
	else
	{
		printf("intRBTree remove key %d fail\n", intRemove);
	}
	if (!RBTreeEmpty(&intRBTree))
	{
		printf("intRBTree size is %d\n", RBTreeSize(&intRBTree));
		printf("intRBTree height is %d\n", RBTreeHeight(&intRBTree));
		RBTreeTravInRec(&intRBTree, IntTraverse);
	}

	int intSearch = 2;
	RBTREENODE *node = RBTreeSearch(&intRBTree, &intSearch);
	if (NULL != node)
	{
		printf("key %d is in intRBTree\n", intSearch);
	}
	else
	{
		printf("key %d is not in intRBTree\n", intSearch);
	}
	intSearch = 11;
	node = RBTreeSearch(&intRBTree, &intSearch);
	if (NULL != node)
	{
		printf("key %d is in intRBTree\n", intSearch);
	}
	else
	{
		printf("key %d is not in intRBTree\n", intSearch);
	}
	RBTreeDispose(&intRBTree);
	
	printf("\n\n");

	RBTREE stringRBTree;
	RBTreeNew(&stringRBTree, sizeof(char *), StringCmp, StringFree);
	char *name1 = strdup("pc");
	char *name2 = strdup("pcwl513");
	char *name3 = strdup("pcpc");
	char *name4 = strdup("jerry");
	char *name5 = strdup("jerry.peng");
	char *name6 = strdup("yanglupu");
	char *name7 = strdup("zhanglei");
	char *name8 = strdup("lishanke");
	RBTreeInsert(&stringRBTree, &name1);
	RBTreeInsert(&stringRBTree, &name2);
	RBTreeInsert(&stringRBTree, &name3);
	RBTreeInsert(&stringRBTree, &name4);
	RBTreeInsert(&stringRBTree, &name5);
	RBTreeInsert(&stringRBTree, &name6);
	RBTreeInsert(&stringRBTree, &name7);
	RBTreeInsert(&stringRBTree, &name8);
	if (!RBTreeEmpty(&stringRBTree))
	{
		printf("stringRBTree size is %d\n", RBTreeSize(&stringRBTree));
		printf("stringRBTree height is %d\n", RBTreeHeight(&stringRBTree));
		RBTreeTravIn(&stringRBTree, StringTraverse);
	}

	char *strRemove = "pcpc";
	if (0 == RBTreeRemove(&stringRBTree, &strRemove))
	{
		printf("stringRBTree remove key %s success\n", strRemove);
	}
	else
	{
		printf("stringRBTree remove key %s fail\n", strRemove);
	}
	if (0 == RBTreeRemove(&stringRBTree, &strRemove))
	{
		printf("stringRBTree remove key %s success\n", strRemove);
	}
	else
	{
		printf("stringRBTree remove key %s fail\n", strRemove);
	}
	if (!RBTreeEmpty(&stringRBTree))
	{
		printf("stringRBTree size is %d\n", RBTreeSize(&stringRBTree));
		printf("stringRBTree height is %d\n", RBTreeHeight(&stringRBTree));
		RBTreeTravInRec(&stringRBTree, StringTraverse);
	}

	char *strSearch = "yanglupu";
	node = RBTreeSearch(&stringRBTree, &strSearch);
	if (NULL != node)
	{
		printf("key %s is in stringRBTree\n", strSearch);
	}
	else
	{
		printf("key %s is not in stringRBTree\n", strSearch);
	}
	strSearch = "123";
	node = RBTreeSearch(&stringRBTree, &strSearch);
	if (NULL != node)
	{
		printf("key %s is in stringRBTree\n", strSearch);
	}
	else
	{
		printf("key %s is not in stringRBTree\n", strSearch);
	}
	RBTreeDispose(&stringRBTree);
	return 0;
}