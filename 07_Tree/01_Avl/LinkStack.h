/* LinkStack.h */
#ifndef _LINK_STACK_H
#define _LINK_STACK_H

typedef void StackFree(void *);

typedef struct stack_node
{
	struct stack_node *next;
	char key[0];
}STACKNODE;

typedef struct 
{
	STACKNODE head;
	int keySize;
	int size;
	StackFree *freeFn;
}STACK;

//链栈的初始化
void StackNew(STACK *s, int keySize, StackFree *freeFn);
//链栈的销毁
void StackDispose(STACK *s);
//入栈
int StackPush(STACK *s, const void *e);
//出栈
int StackPop(STACK *s, void *e);
//栈的判空
int StackEmpty(STACK *s);
//栈中节点数量
int StackSize(STACK *s);
//获取栈顶元素
int StackTop(STACK *s, void *e);
#endif