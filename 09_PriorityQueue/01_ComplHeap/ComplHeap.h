/* ComplHeap.h */
#ifndef _COMPL_HEAP_H
#define _COMPL_HEAP_H

typedef int  ComplHeapCmp(const void *, const void *);
typedef void ComplHeapFree(void *);

//顺序表表示完全二叉堆
typedef struct
{
	void *elems;//顺序表的起始地址
	int elemSize;//每个元素的大小
	long long size;//当前顺序表元素的个数
	long long initCap;//顺序表初始容量
	long long capacity;//当前顺序表容量
	ComplHeapCmp *cmpFn;
	ComplHeapFree *freeFn;
}PQUEUE;

//优先级队列初始化
void PQueueNew(PQUEUE *pq, int elemSize, long long initCap, ComplHeapCmp *cmpFn, ComplHeapFree *freeFn);
//获取优先级队列元素个数
long long PQueueSize(PQUEUE *pq);
//优先级队列判空，返回值：0--非空，!0--空
int PQueueEmpty(PQUEUE *pq);
//清空优先级队列所有元素
void PQueueMakeEmpty(PQUEUE *pq);
//优先级队列销毁
void PQueueDispose(PQUEUE *pq);
//获取当前优先级最大的元素
int PQueueGetMax(PQUEUE *pq, void *e);
//优先级队列插入关键码e，返回值：0--成功，!0--失败
int PQueueInsert(PQUEUE *pq, const void *e);
//优先级队列删除优先级最大的元素
int PQueueDeleteMax(PQUEUE *pq);
//批量建堆
void PQHeapify(PQUEUE *pq);
#endif