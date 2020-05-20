/* ClosedHash.c */
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include "ClosedHash.h"

static int IntCmp(const void *keyAddr, const void *dataAddr)
{
	int *p1 = (int *)keyAddr;
	int *p2 = (int *)dataAddr;
	return (*p1 - *p2);
}

//散列表初始化
void HashNew(HASH *h, int capacity, int keySize, int valSize, HashFunc *hashFn, HashCollide* collideFn, HashCmp *cmpFn, HashFree *freeFn)
{
	assert(0 < capacity);
	assert(0 < keySize);
	assert(0 <= valSize);
	assert(NULL != hashFn);
	assert(NULL != collideFn);
	h->capacity = capacity;
	h->keySize = keySize;
	h->valSize = valSize;
	h->size = 0;
	h->hashFn = hashFn;
	h->collideFn = collideFn;
	h->cmpFn = cmpFn;
	h->freeFn = freeFn;
	VectorNew(&(h->vEntry), keySize + valSize, capacity, 0, cmpFn, freeFn);
	int i;
	//将所有节点的key置为-1
	void *initEntry = malloc(keySize + valSize);
	assert(NULL != initEntry);
	memset(initEntry, -1, keySize + valSize);
	for (i = 0; i < capacity; i ++)
	{
		VectorInsertByPos(&(h->vEntry), initEntry, i);
	}
	free(initEntry);
	VectorNew(&(h->vLazy), sizeof(int), capacity, 0, IntCmp, NULL);
	//将所有节点的lazyFlag置0
	int initLazy = 0;
	for (i = 0; i < capacity; i ++)
	{
		VectorInsertByPos(&(h->vLazy), &initLazy, i);
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
	VectorDispose(&(h->vEntry));
	VectorDispose(&(h->vLazy));
	h->size = 0;
}

//沿关键码对应的查找链，找到与之匹配的桶
static int probe4Hit(HASH *h, int hashKey, const void *e)
{
	int r = hashKey;
	void *fEmpty = malloc(h->keySize);
	assert(NULL != fEmpty);
	memset(fEmpty, -1, h->keySize);
	int fLazy = 1;
	int count = 0;
	//跳过所有冲突的桶，以及带懒惰删除标记的桶
	while ((!VectorFind(&(h->vEntry), r, fEmpty) && !VectorFind(&(h->vEntry), r, e))
		|| (VectorFind(&(h->vEntry), r, fEmpty) && (VectorFind(&(h->vLazy), r, &fLazy))))
	{
		count ++;
		r = h->collideFn(r, count);
	}
	free(fEmpty);
	return r;
}

//散列表读取
void *HashGet(HASH *h, const void *e)
{
	int hashKey = h->hashFn(e);
	hashKey = probe4Hit(h, hashKey, e);
	if (!VectorFind(&(h->vEntry), hashKey, e))
	{
		return NULL;
	}
	return VectorGetByPos(&(h->vEntry), hashKey);
}

static int probe4Free(HASH *h, int hashKey)
{
	int r = hashKey;
	void *fEmpty = malloc(h->keySize);
	assert(NULL != fEmpty);
	memset(fEmpty, -1, h->keySize);
	int fLazy = 1;
	int count = 0;
	//跳过所有冲突的桶，以及带懒惰删除标记的桶
	while (!VectorFind(&(h->vEntry), r, fEmpty))
	{
		count ++;
		r = h->collideFn(r, count);
	}
	free(fEmpty);
	return r;
}

//散列表插入
int HashPut(HASH *h, const void *e, const void *val)
{
	int hashKey = h->hashFn(e);
	hashKey = probe4Hit(h, hashKey, e);
	if (VectorFind(&(h->vEntry), hashKey, e))
	{
		return -1;
	}
	hashKey = probe4Free(h, hashKey);
	void *data = malloc(h->keySize + h->valSize);
	assert(NULL != data);
	memcpy(data, e, h->keySize);
	memcpy((char *)data + h->keySize, val, h->valSize);
	VectorUpdate(&(h->vEntry), hashKey, data);
	free(data);
	h->size ++;
	return 0;
}

//散列表删除
int HashRemove(HASH *h, void *e)
{
	int hashKey = h->hashFn(e);
	hashKey = probe4Hit(h, hashKey, e);
	if (!VectorFind(&(h->vEntry), hashKey, e))
	{
		return -1;
	}
	void *initEntry = malloc(h->keySize + h->valSize);
	assert(NULL != initEntry);
	memset(initEntry, -1, h->keySize + h->valSize);
	VectorRemoveByPos(&(h->vEntry), hashKey);
	VectorInsertByPos(&(h->vEntry), initEntry, hashKey);
	free(initEntry);
	int fRemove = 1;
	VectorUpdate(&(h->vLazy), hashKey, &fRemove);
	h->size --;
	return 0;
}

//重散列
static void HashRehash(HASH *hN, HASH *hO)
{
	int i = 0;
	void *fEmpty = malloc(hO->keySize);
	assert(fEmpty);
	memset(fEmpty, -1, hO->keySize);
	for (; i < hO->capacity; i ++)
	{
		if (!VectorFind(&(hO->vEntry), i, fEmpty))
		{
			void *dataGet = VectorGetByPos(&(hO->vEntry), i);
			HashPut(hN, dataGet, dataGet + hO->keySize);
		}
	}
}