/* DListTest.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "DList.h"

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
static void StringFree(void *keyAddr)
{
	char *p = *(char **)keyAddr;
	free(p);
}
static void StringTraverse(void *keyAddr, void *outData)
{
	char *p = *(char **)keyAddr;
	printf("%s\n", p);
}

int main()
{
	LIST intList;
	ListNew(&intList, sizeof(int), IntCmp, NULL);
	int i = 0;
	for (; i < 10; i++)
	{
		ListInsert(&intList, &i, LIST_FORWARD);
	}
	if (!ListEmpty(&intList))
	{
		printf("intList size is %d\n", ListSize(&intList));
		ListTraverse(&intList, IntTraverse, NULL);		
	}
	int intRemove = 3;
	if (0 == ListRemove(&intList, &intRemove))
	{
		printf("intList remove %d success\n", intRemove);
	}
	else
	{
		printf("intList remove %d fail\n", intRemove);		
	}
	if (0 == ListRemove(&intList, &intRemove))
	{
		printf("intList remove %d success\n", intRemove);
	}
	else
	{
		printf("intList remove %d fail\n", intRemove);		
	}
	if (!ListEmpty(&intList))
	{
		printf("intList size is %d\n", ListSize(&intList));
		ListTraverse(&intList, IntTraverse, NULL);		
	}
	int intSearch = 2;
	if (NULL != ListSearch(&intList, &intSearch))
	{
		printf("data %d is in intList\n", intSearch);
	}
	else
	{
		printf("data %d is not in intList\n", intSearch);		
	}
	intSearch = 11;
	if (NULL != ListSearch(&intList, &intSearch))
	{
		printf("data %d is in intList\n", intSearch);
	}
	else
	{
		printf("data %d is not in intList\n", intSearch);		
	}
	ListDispose(&intList);
	
	printf("\n\n\n");

	LIST stringList;
	ListNew(&stringList, sizeof(char *), StringCmp, StringFree);
	char *name1 = strdup("pc");
	char *name2 = strdup("pcwl513");
	char *name3 = strdup("pcpc");
	char *name4 = strdup("jerry");
	char *name5 = strdup("jerry.peng");
	char *name6 = strdup("yanglupu");
	char *name7 = strdup("zhanglei");
	char *name8 = strdup("lishanke");
	ListInsert(&stringList, &name1, !LIST_FORWARD);
	ListInsert(&stringList, &name2, !LIST_FORWARD);
	ListInsert(&stringList, &name3, !LIST_FORWARD);
	ListInsert(&stringList, &name4, !LIST_FORWARD);
	ListInsert(&stringList, &name5, !LIST_FORWARD);
	ListInsert(&stringList, &name6, !LIST_FORWARD);
	ListInsert(&stringList, &name7, !LIST_FORWARD);
	ListInsert(&stringList, &name8, !LIST_FORWARD);
	if (!ListEmpty(&stringList))
	{
		printf("stringList size is %d\n", ListSize(&stringList));
		ListTraverse(&stringList, StringTraverse, NULL);		
	}
	char *strRemove = "pcpc";
	if (0 == ListRemove(&stringList, &strRemove))
	{
		printf("stringList remove %s success\n", strRemove);
	}
	else
	{
		printf("stringList remove %s fail\n", strRemove);		
	}
	if (0 == ListRemove(&stringList, &strRemove))
	{
		printf("stringList remove %s success\n", strRemove);
	}
	else
	{
		printf("stringList remove %s fail\n", strRemove);		
	}
	if (!ListEmpty(&stringList))
	{
		printf("stringList size is %d\n", ListSize(&stringList));
		ListTraverse(&stringList, StringTraverse, NULL);		
	}
	char *strSearch = "yanglupu";
	if (NULL != ListSearch(&stringList, &strSearch))
	{
		printf("data %s is in stringList\n", strSearch);
	}
	else
	{
		printf("data %s is not in stringList\n", strSearch);		
	}
	strSearch = "123";
	if (NULL != ListSearch(&stringList, &strSearch))
	{
		printf("data %s is in stringList\n", strSearch);
	}
	else
	{
		printf("data %s is not in stringList\n", strSearch);		
	}
	ListDispose(&stringList);
	return 0;
}