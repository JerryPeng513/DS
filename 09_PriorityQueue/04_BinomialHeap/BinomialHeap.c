/* BinomialHeap.c */
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include "BinomialHeap.h"
#include "LinkQueue.h"

static BINOMIALNODE *nodeNew(int keySize, const void *e)
{
	BINOMIALNODE *newNode = (BINOMIALNODE *)malloc(sizeof(BINOMIALNODE) + keySize);
	if (NULL == newNode)
	{
		return NULL;
	}
	newNode->parent = NULL;
	newNode->lc = NULL;
	newNode->br = NULL;
	newNode->degree = 0;
	memcpy(newNode->key, e, keySize);
	return newNode;
}

static void nodeDispose(BINOMIALNODE *node, BinomialHeapFree *freeFn)
{
	if (NULL != freeFn)
	{
		freeFn(node->key);
	}
	free(node);
}

//PQueue初始化
void PQueueNew(PQUEUE *pq, int keySize, BinomialHeapCmp *cmpFn, BinomialHeapFree *freeFn)
{
	assert(0 < keySize);
	assert(NULL != cmpFn);
	pq->keySize = keySize;
	pq->size = 0;
	pq->cmpFn = cmpFn;
	pq->freeFn = freeFn;
	pq->root = NULL;
}

//PQueue判空
int PQueueEmpty(PQUEUE *pq)
{
	return (0 == pq->size);
}

//PQueue规模
int PQueueSize(PQUEUE *pq)
{
	return pq->size;
}

static void addNode2Queue(void *elemAddr, void *outData)
{
	QUEUE *q = (QUEUE *)outData;
	if (NULL == q)
	{
		return ;
	}
	QueueEn(q, elemAddr);
}

//PQueue销毁
void PQueueDispose(PQUEUE *pq)
{
	if (PQueueEmpty(pq))
	{
		return ;
	}
	QUEUE nodeQueue;
	QueueNew(&nodeQueue, sizeof(BINOMIALNODE *), NULL);
	BINOMIALNODE *node = pq->root;
	QueueEn(&nodeQueue, &node);
	while (!QueueEmpty(&nodeQueue))
	{
		QueueDe(&nodeQueue, &node);
		if (NULL != node->lc)
		{
			QueueEn(&nodeQueue, &(node->lc));
		}
		if (NULL != node->rc)
		{
			QueueEn(&nodeQueue, &(node->rc));
		}
		nodeDispose(node, pq->freeFn);
	}
	QueueDispose(&nodeQueue);
	pq->root = NULL;
	pq->size = 0;
}

//获取当前优先级最大的元素
int PQueueGetMax(PQUEUE *pq, void *e)
{
	if (PQueueEmpty(pq) || NULL == e)
	{
		return -1;
	}
	memcpy(e, pq->root->key, pq->keySize);
	return 0;
}

//将rootA、rootB中的根表合并成一个按度数递增的链表，返回合并后的根节点
static binomialMerge(BINOMIALNODE *rootA, BINOMIALNODE *rootB)
{
	//head为指向新堆根结点
	BinomialNode* head = NULL;
	BinomialNode** pos = &head; 
    
	while (rootA && rootB)
    {
        if (rootA->degree < rootB->degree)
        {
            *pos = rootA;
            rootA = rootA->next;
        } 
        else 
        {
            *pos = rootB;
            rootB = rootB->next;
        }
        pos = &(*pos)->next;
    }
    if (rootA)
	{
        *pos = rootA;
    }
	else
	{
        *pos = rootB;
	}
    return head;
}

//关联两个二项堆：将child作为heap的左孩子
static void binomialLink(BINOMIALNODE *child, BINOMIALNODE *heap)
{
	child->parent = heap;
	child->next = heap->child;
	heap->child = child;
	heap->degree ++;
}

//合并以rootA和rootB为根节点的两个二项堆，并返回合并后的堆
static BINOMIALNODE *heapMerge(BINOMIALNODE *rootA, BINOMIALNODE *rootB, BinomialHeapCmp *cmpFn)
{
	BINOMIALNODE *heap = NULL;
	BINOMIALNODE *xPrev = NULL;
	BINOMIALNODE *x = NULL;
	BINOMIALNODE *xNext = NULL;
	
	//将rootA、rootB为头结点的根表合并成一个按度数递增的链表heap
	heap = binomialMerge(rootA, rootB);
	if (heap == NULL)
	{
		return NULL;
	}
	
	while (a && b)
	{
		if (a->degree < b->degree)
		{
			head = a;
			a = a->next;
		}
		else
		{
			head = b;
			b = b->next;
		}
		
	}
	return ;
}

//优先级队列插入关键码e，返回值：0--成功，!0--失败
int PQueueInsert(PQUEUE *pq, const void *e)
{
	BINOMIALNODE *newNode = nodeNew(pq->keySize, e);
	if (NULL == newNode)
	{
		return -1;
	}
	pq->root = merge(pq, pq->root, newNode);
	pq->root->parent = NULL;
	pq->size ++;
	return 0;
}

//优先级队列删除优先级最大的元素
int PQueueDeleteMax(PQUEUE *pq)
{
	if (PQueueEmpty(pq))
	{
		return -1;
	}
	BINOMIALNODE *lHeap = pq->root->lc;
	BINOMIALNODE *rHeap = pq->root->rc;
	nodeDispose(pq->root, pq->freeFn);
	pq->size --;
	pq->root = merge(pq, lHeap, rHeap);
	if (NULL != pq->root)
	{
		pq->root->parent = NULL;
	}
	return 0;
}