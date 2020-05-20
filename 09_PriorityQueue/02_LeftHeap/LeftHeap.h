/* LeftHeap.h */
#ifndef _LEFT_HEAP_H
#define _LEFT_HEAP_H

//0--相同，>0--dataAddr<keyAddr，<0--dataAddr>keyAddr
typedef int  LeftHeapCmp(const void *keyAddr, const void *dataAddr);
typedef void LeftHeapFree(void *);

typedef struct bin_node
{
	struct bin_node *parent;
	struct bin_node *lc;
	struct bin_node *rc;
	int npl;
	char key[0];
}BINNODE;

typedef struct
{
	BINNODE *root;
	int size;
	int keySize;
	LeftHeapCmp *cmpFn;
	LeftHeapFree *freeFn;
}PQUEUE;

//PQueue初始化
void PQueueNew(PQUEUE *pq, int keySize, LeftHeapCmp *cmpFn, LeftHeapFree *freeFn);
//PQueue判空
int PQueueEmpty(PQUEUE *pq);
//PQueue规模
int PQueueSize(PQUEUE *pq);
//PQueue销毁
void PQueueDispose(PQUEUE *pq);
//获取当前优先级最大的元素
int PQueueGetMax(PQUEUE *pq, void *e);
//优先级队列插入关键码e，返回值：0--成功，!0--失败
int PQueueInsert(PQUEUE *pq, const void *e);
//优先级队列删除优先级最大的元素
int PQueueDeleteMax(PQUEUE *pq);
#endif