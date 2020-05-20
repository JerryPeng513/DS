/* LinkQueue.h */
#ifndef _LINK_QUEUE_H
#define _LINK_QUEUE_H

typedef void QueueFree(void *);

typedef struct queue_node
{
	struct queue_node *prev;
	struct queue_node *next;
	char key[0];
}QUEUENODE;

typedef struct 
{
	QUEUENODE head;
	int size;
	int keySize;
	QueueFree *freeFn;
}QUEUE;

//队列初始化
void QueueNew(QUEUE *q, int keySize, QueueFree *freeFn);
//队列销毁
void QueueDispose(QUEUE *q);
//入队操作，新节点插入到队尾
int QueueEn(QUEUE *q, const void *e);
//出队操作，节点从队头出队
int QueueDe(QUEUE *q, void *e);
//获取队头元素
int QueueTop(QUEUE *q, void *e);
//获取队尾元素
int QueueRear(QUEUE *q, void *e);
//队列判空
int QueueEmpty(QUEUE *q);
//队列节点数量
int QueueSize(QUEUE *q);
#endif