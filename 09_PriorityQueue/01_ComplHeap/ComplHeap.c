/* ComplHeap.c */
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include "ComplHeap.h"

//判断PQ[i]是否合法
#define InHeap(n, i)       (((-1) < (i)) && ((i) < (n)))
//PQ[i]的父亲节点
#define Parent(i)          ((i - 1) >> 1)
//最后一个内部节点（末节点的父亲）
#define LastInternal(n)    Parent(n - 1)
//PQ[i]的左孩子
#define LChild(i)          (1 + ((i) << 1))
//PQ[i]的右孩子
#define RChild(i)          ((1 + (i)) << 1)
//判断PQ[i]是否有父亲
#define ParentValid(i)     (0 < i)
//判断PQ[i]是否有一个（左）孩子
#define LChildVaild(n, i)  InHeap(n, LChild(i))
//判断PQ[i]是否有两个孩子
#define RChildVaild(n, i)  InHeap(n, RChild(i))

//优先级队列堆空间倍增
static void PQueueGrow(PQUEUE *pq)
{
	pq->capacity = (pq->capacity) << 1;
	pq->elems = realloc(pq->elems, pq->elemSize * pq->capacity);
	assert(pq->elems);
}

//优先级队列堆空间倍减
static void PQueueReduce(PQUEUE *pq)
{
	if ((pq->capacity >> 1) < pq->initCap)
	{
		return ;
	}
	pq->capacity = (pq->capacity) >> 1;
	pq->elems = realloc(pq->elems, pq->elemSize * pq->capacity);
	assert(pq->elems);
}


//优先级队列初始化
void PQueueNew(PQUEUE *pq, int elemSize, long long initCap, ComplHeapCmp *cmpFn, ComplHeapFree *freeFn)
{
	assert(0 < elemSize);
	assert(0 < initCap);
	assert(NULL != cmpFn);
	pq->elemSize = elemSize;
	pq->initCap = initCap;
	pq->capacity = initCap;
	pq->cmpFn = cmpFn;
	pq->freeFn = freeFn;
	pq->size = 0;
	pq->elems = malloc(elemSize * initCap);
	assert(NULL != pq->elems);
}

//获取优先级队列元素个数
long long PQueueSize(PQUEUE *pq)
{
	return pq->size;
}

//优先级队列判空，返回值：0--非空，!0--空
int PQueueEmpty(PQUEUE *pq)
{
	return (0 == pq->size);
}

//清空优先级队列所有元素
void PQueueMakeEmpty(PQUEUE *pq)
{
	if (NULL != pq->freeFn)
	{
		int i;
		for(i = 0; i < pq->size; i++)
		{
			pq->freeFn((char *)pq->elems + i * pq->elemSize);
		}
	}
	pq->size = 0;
	pq->elems = realloc(pq->elems, pq->elemSize * pq->initCap);
	assert(NULL != pq->elems);
}

//优先级队列销毁
void PQueueDispose(PQUEUE *pq)
{
	if (NULL != pq->freeFn)
	{
		int i;
		for(i = 0; i < pq->size; i++)
		{
			pq->freeFn((char *)pq->elems + i * pq->elemSize);
		}
	}
	free(pq->elems);
	pq->elems = NULL;
	pq->size = 0;
}

//获取当前优先级最大的元素
int PQueueGetMax(PQUEUE *pq, void *e)
{
	if (PQueueEmpty(pq) || NULL == e)
	{
		return -1;
	}
	memcpy(e, pq->elems, pq->elemSize);
	return 0;
}

//从第pos个节点开始上虑
static void percolateUp(PQUEUE *pq, long long pos)
{
	long long c = pos; //current node
	long long p = Parent(c); //parent node	
	void *tmp = malloc(pq->elemSize);
	assert(NULL != tmp);
	memcpy(tmp, (char *)pq->elems + c * pq->elemSize, pq->elemSize);
	void *cAddr, *pAddr;
	while (ParentValid(c))
	{
		cAddr = (char *)pq->elems + c * pq->elemSize;
		pAddr = (char *)pq->elems + p * pq->elemSize;
		//父节点的优先级低于待上虑节点的优先级继续上虑
		if (pq->cmpFn(tmp, pAddr) > 0)
		{
			memcpy(cAddr, pAddr, pq->elemSize);
			c = p;
			p = Parent(p);
		}
		else
		{
			break;
		}
	}
	cAddr = (char *)pq->elems + c * pq->elemSize;
	memcpy(cAddr, tmp, pq->elemSize);
	free(tmp);
}

//优先级队列插入关键码e，返回值：0--成功，!0--失败
int PQueueInsert(PQUEUE *pq, const void *e)
{
	//当前队列满了
	if (pq->capacity == pq->size)
	{
		return -1;
		//队列达到扩展上限，插入失败
		//if ((pq->capacity << 1) < 0)
		//{
		//	return -1;
		//}
		//else
		//{
		//	PQueueGrow(pq);
		//}
	}
	memcpy((char *)pq->elems + pq->size * pq->elemSize, e, pq->elemSize);
	pq->size ++;
	percolateUp(pq, pq->size - 1);
	return 0;
}

static void percolateDown(PQUEUE *pq, long long begin, long long end)
{
	long long cur = begin; //current node
	long long left = LChild(cur); //left child position	
	void *tmp = malloc(pq->elemSize);
	assert(NULL != tmp);
	memcpy(tmp, pq->elems, pq->elemSize);
	for (; left < end; cur = left, left = LChild(left))
	{
		void *lAddr = (char *)pq->elems + left * pq->elemSize;
		if (left + 1 < end)
		{
			void *rAddr = (char *)pq->elems + (left + 1) * pq->elemSize;
			if (pq->cmpFn(rAddr, lAddr) > 0)
			{
				lAddr = rAddr;
				left ++;
			}
		}
		//待下虑节点的优先级低于两个孩子节点中最大的优先级继续下虑
		if (pq->cmpFn(lAddr, tmp) > 0)
		{
			void *cAddr = (char *)pq->elems + cur * pq->elemSize;
			memcpy(cAddr, lAddr, pq->elemSize);
		}
		else
		{
			break;
		}
	}
	void *cAddr = (char *)pq->elems + cur * pq->elemSize;
	memcpy(cAddr, tmp, pq->elemSize);
	free(tmp);
}

//优先级队列删除优先级最大的元素
int PQueueDeleteMax(PQUEUE *pq)
{
	if (PQueueEmpty(pq))
	{
		return -1;
	}
	if (NULL != pq->freeFn)
	{
		pq->freeFn(pq->elems);
	}
	void *last = (char *)pq->elems + (pq->size - 1) * pq->elemSize;
	if (PQueueSize(pq) > 1)
	{
		memcpy(pq->elems, last, pq->elemSize); //将最后一个元素覆盖到第一个元素
	}
	pq->size --;
	percolateDown(pq, 0, pq->size);
	//队列进行紧缩
	//if  (pq->size <= ((pq->capacity >> 1) - REDUCTION_THRESHOLD) && pq->capacity > pq->initCap)
	//{
	//	PQueueReduce(pq);
	//}
	return 0;
}

//批量建堆，Floyd建堆算法，时间复杂度O(n)
void PQHeapify(PQUEUE *pq)
{
	int i = LastInternal(pq->size);
	for (; InHeap(pq->size, i); i --)
	{
		percolateDown(pq, i, pq->size);
	}
}