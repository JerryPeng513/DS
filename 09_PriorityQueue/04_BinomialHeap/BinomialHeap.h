/* BinomialHeap.h */
#ifndef _BINOMIAL_HEAP_H
#define _BINOMIAL_HEAP_H

//0--相同，>0--dataAddr<keyAddr，<0--dataAddr>keyAddr
typedef int  BinomialHeapCmp(const void *keyAddr, const void *dataAddr);
typedef void BinomialHeapFree(void *);

typedef struct binomial_node
{
	struct binomial_node *parent;
	struct binomial_node *lc; //左孩子
	struct binomial_node *br; //兄弟
	int degree;
	char key[0];
}BINOMIALNODE;

typedef struct
{
	BINOMIALNODE *root;
	int size;
	int keySize;
	BinomialHeapCmp *cmpFn;
	BinomialHeapFree *freeFn;
}PQUEUE;

//PQueue初始化
void PQueueNew(PQUEUE *pq, int keySize, BinomialHeapCmp *cmpFn, BinomialHeapFree *freeFn);
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