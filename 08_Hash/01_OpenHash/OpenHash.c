/* OpenHash.c */
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include "OpenHash.h"

//散列表初始化
void HashNew(HASH *h, int capacity, int keySize, int valSize, HashFunc *hashFn, HashCmp *cmpFn, HashFree *freeFn)
{
	assert(0 < capacity);
	assert(0 < keySize);
	assert(0 <= valSize);
	assert(NULL != hashFn);
	h->capacity = capacity;
	h->keySize = keySize;
	h->valSize = valSize;
	h->size = 0;
	h->hashFn = hashFn;
	h->cmpFn = cmpFn;
	h->freeFn = freeFn;
	h->listSet = (LIST *)malloc(sizeof(LIST) * capacity);
	assert(h->listSet);
	int i = 0;
	for (; i < capacity; i ++)
	{
		ListNew(&(h->listSet[i]), keySize + valSize, cmpFn, freeFn);
	}
}

//获取散列表数据数量
int HashSize(HASH *h)
{
	return h->size;
}

//获取散列表容量
int HashCapacity(HASH *h)
{
	return h->capacity;
}

//散列表销毁
void HashDispose(HASH *h)
{
	int i = 0;
	for (; i < h->capacity; i ++)
	{
		ListDispose(&(h->listSet[i]));
	}
	free(h->listSet);
	h->listSet = NULL;
	h->size = 0;
}

//散列表读取
void *HashGet(HASH *h, const void *e)
{
	int hashKey = h->hashFn(e);
	return ListSearch(&(h->listSet[hashKey]), e);
}

//散列表插入
int HashPut(HASH *h, const void *e, const void *val)
{
	int hashKey = h->hashFn(e);
	if (NULL != ListSearch(&(h->listSet[hashKey]), e))
	{
		return -1;
	}
	void *data = malloc(h->keySize + h->valSize);
	assert(NULL != data);
	memcpy(data, e, h->keySize);
	memcpy((char *)data + h->keySize, val, h->valSize);
	if (0 != ListInsert(&(h->listSet[hashKey]), data, 0))
	{
		return -1;
	}
	free(data);
	h->size ++;
	return 0;
}

//散列表删除
int HashRemove(HASH *h, void *e)
{
	int hashKey = h->hashFn(e);
	if (NULL == ListSearch(&(h->listSet[hashKey]), e))
	{
		return -1;
	}
	if (0 != ListRemove(&(h->listSet[hashKey]), e))
	{
		return -1;
	}
	h->size --;
	return 0;
}