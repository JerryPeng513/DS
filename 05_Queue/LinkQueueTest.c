/*LinkQueueTest.c*/
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include "LinkQueue.h"

static void StringFree(void *elemAddr)
{
	free(*(char **)elemAddr);
}

int main()
{
	int i;
	const char friends[][5] = {"Al", "Bob", "Carl"};
	QUEUE stringQueue;
	QueueNew(&stringQueue, sizeof(char *), StringFree);
	for(i = 0; i < 3; i++)
	{
		char *copy = strdup(friends[i]);
		QueueEn(&stringQueue, &copy);
	}
	char *name;
	for(i = 0; i < 3; i++)
	{
		QueueDe(&stringQueue, &name);
		printf("%s\n", name);
	}
	QueueDispose(&stringQueue);
	return 0;
}
