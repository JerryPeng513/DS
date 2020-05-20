/* SplayTest.c */
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include "Splay.h"

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
	SPLAYTREE intSplay;
	SplayNew(&intSplay, sizeof(int), IntCmp, NULL);
	int i = 0;
	for (; i < 10; i++)
	{
		SplayInsert(&intSplay, &i);
	}
	if (!SplayEmpty(&intSplay))
	{
		printf("size of intSplay is %d\n", SplaySize(&intSplay));
		SplayTravIn(&intSplay, IntTraverse);
	}

	int intRemve = 1;
	if (0 == SplayRemove(&intSplay, &intRemve))
	{
		printf("intSplay remove key %d success\n", intRemve);
	}
	else
	{
		printf("intSplay remove key %d fail\n", intRemve);
	}
	if (0 == SplayRemove(&intSplay, &intRemve))
	{
		printf("intSplay remove key %d success\n", intRemve);
	}
	else
	{
		printf("intSplay remove key %d fail\n", intRemve);
	}
	if (!SplayEmpty(&intSplay))
	{
		printf("size of intSplay is %d\n", SplaySize(&intSplay));
		SplayTravInRec(&intSplay, IntTraverse);
	}

	int intSearch = 2;
	SPLAYNODE *node = SplaySearch(&intSplay, &intSearch);
	if (SplayFind(&intSplay, node, &intSearch))
	{
		printf("data %d is in intSplay\n", intSearch);
	}
	else
	{
		printf("data %d is not in intSplay\n", intSearch);		
	}
	intSearch = 12;
	node = SplaySearch(&intSplay, &intSearch);
	if (SplayFind(&intSplay, node, &intSearch))
	{
		printf("data %d is in intSplay\n", intSearch);
	}
	else
	{
		printf("data %d is not in intSplay\n", intSearch);		
	}
	SplayDispose(&intSplay);
		
	printf("\n\n");

	SPLAYTREE stringSplay;
	SplayNew(&stringSplay, sizeof(char *), StringCmp, StringFree);
	
	char *name1 = strdup("pc");
	char *name2 = strdup("pcwl513");
	char *name3 = strdup("pcpc");
	char *name4 = strdup("jerry");
	char *name5 = strdup("jerry.peng");
	char *name6 = strdup("yanglupu");
	char *name7 = strdup("zhanglei");
	char *name8 = strdup("lishanke");
	SplayInsert(&stringSplay, &name1);
	SplayInsert(&stringSplay, &name2);
	SplayInsert(&stringSplay, &name3);
	SplayInsert(&stringSplay, &name4);
	SplayInsert(&stringSplay, &name5);
	SplayInsert(&stringSplay, &name6);
	SplayInsert(&stringSplay, &name7);
	SplayInsert(&stringSplay, &name8);
	if (!SplayEmpty(&stringSplay))
	{
		printf("size of stringSplay is %d\n", SplaySize(&stringSplay));
		SplayTravIn(&stringSplay, StringTraverse);
	}

	char *strRemove = "pcpc";
	if (0 == SplayRemove(&stringSplay, &strRemove))
	{
		printf("stringSplay remove key %s success\n", strRemove);
	}
	else
	{
		printf("stringSplay remove key %s fail\n", strRemove);	
	}
	if (0 == SplayRemove(&stringSplay, &strRemove))
	{
		printf("stringSplay remove key %s success\n", strRemove);
	}
	else
	{
		printf("stringSplay remove key %s fail\n", strRemove);	
	}
	if (!SplayEmpty(&stringSplay))
	{
		printf("size of stringSplay is %d\n", SplaySize(&stringSplay));
		SplayTravInRec(&stringSplay, StringTraverse);
	}

	char *strSearch = "yanglupu";
	node = SplaySearch(&stringSplay, &strSearch);
	if (SplayFind(&stringSplay, node, &strSearch))
	{
		printf("data %s is in intSplay\n", strSearch);
	}
	else
	{
		printf("data %s is not in intSplay\n", strSearch);		
	}
	strSearch = "123";
	node = SplaySearch(&stringSplay, &strSearch);
	if (SplayFind(&stringSplay, node, &strSearch))
	{
		printf("data %s is in intSplay\n", strSearch);
	}
	else
	{
		printf("data %s is not in intSplay\n", strSearch);		
	}
	SplayDispose(&stringSplay);
	return 0;
}