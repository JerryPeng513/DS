/* Avl.c */
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include "Avl.h"
#include "LinkStack.h"
#include "LinkQueue.h"

/* x表示值，p表示指针 */
#define IsRoot(x)       (!((x).parent))
#define IsLChild(x)     (!IsRoot(x) && (&(x) == (x).parent->lc))
#define IsRChild(x)     (!IsRoot(x) && (&(x) == (x).parent->rc))
#define HasParent(x)    (!IsRoot(x))
#define HasLChild(x)    ((x).lc)
#define HasRChild(x)    ((x).rc)
#define HasChild(x)     (HasLChild(x) || HasRChild(x))
#define HasBothChild(x) (HasLChild(x) && HasRChild(x))
#define IsLeaf(x)       (!HasChild(x))
//获取x的兄弟节点
#define Sibling(x)      (IsLChild(x) ? (x).parent->rc : (x).parent->lc)
//获取x的叔叔节点
#define Uncle(x)        (IsLChild(*((x).parent)) ? (x).parent->parent->rc : (x).parent->parent->lc)
//节点高度，空树高度为-1
#define stature(p)      ((p) ? (p)->height : -1)
//理想平衡条件
#define Balanced(x)     (stature((x).lc) == stature((x).rc))
//平衡因子
#define BalFac(x)       (stature((x).lc) - stature((x).rc))
//AVL平衡条件
#define AvlBalanced(x)  ((-2 < BalFac(x)) && (BalFac(x) < 2))
//在左、右孩子中取更高者，在AVL平衡调整前，借此确定重构方案
#define tallerChild(p)  ( \
	stature((p)->lc) > stature((p)->rc) ? (p)->lc : ( /*左高*/ \
	stature((p)->lc) < stature((p)->rc) ? (p)->rc : ( /*右高*/ \
	IsLChild(*(p)) ? (p)->lc : (p)->rc /*等高：与父亲p同侧者(zig-zig或zag-zag)优先*/ \
	) \
   	) \
)

static AVLNODE *nodeNew(int keySize, const void *e)
{
	AVLNODE *newNode = (AVLNODE *)malloc(sizeof(AVLNODE) + keySize);
	if (NULL == newNode)
	{
		return NULL;
	}
	newNode->parent = NULL;
	newNode->lc = NULL;
	newNode->rc = NULL;
	newNode->height = 0;
	memcpy(newNode->key, e, keySize);
	return newNode;
}

static void nodeDispose(AVLNODE *node, AvlFree *freeFn)
{
	if (NULL != freeFn)
	{
		freeFn(node->key);
	}
	free(node);
}

//Avl初始化
void AvlNew(AVLTREE *avlTree, int keySize, AvlCmp *cmpFn, AvlFree *freeFn)
{
	assert(keySize > 0);
	assert(NULL != cmpFn);
	avlTree->root = NULL;
	avlTree->hot = NULL;
	avlTree->size = 0;
	avlTree->keySize = keySize;
	avlTree->cmpFn = cmpFn;
	avlTree->freeFn = freeFn;
}

//Avl判空
int AvlEmpty(AVLTREE *avlTree)
{
	return (avlTree->size == 0);
}

//Avl规模
int AvlSize(AVLTREE *avlTree)
{
	return avlTree->size;
}

//Avl树高度
int AvlHeight(AVLTREE *avlTree)
{
	return avlTree->root->height;
}

//将当前节点及其左侧分支入栈
static goAlongLeftBranch(AVLNODE *node, STACK *s)
{
	AVLNODE *cur = node;
	while (NULL != cur)
	{
		StackPush(s, &cur);
		cur = cur->lc;
	}
}

//Avl销毁
void AvlDispose(AVLTREE *avlTree)
{
	if (AvlEmpty(avlTree))
	{
		return ;
	}
	STACK avlNodeStack;
	//栈中存放avl节点指针
	StackNew(&avlNodeStack, sizeof(AVLNODE *), NULL);	
	AVLNODE *node = avlTree->root, *cur;
	while (1)
	{
		//从当前节点出发，逐批入栈
		goAlongLeftBranch(node, &avlNodeStack);
		//所有节点处理完毕
		if (StackEmpty(&avlNodeStack))
		{
			break;
		}
		//弹出栈顶节点并访问之
		StackPop(&avlNodeStack, &node);
		cur = node;
		node = node->rc; //转向右子树
		nodeDispose(cur, avlTree->freeFn);
	}
	StackDispose(&avlNodeStack);
	avlTree->root = NULL;
	avlTree->hot = NULL;
	avlTree->size = 0;
}

static void visitAlongLeftBranch(AVLNODE *node, STACK *s, AvlTraverseOp *traverseOpFn)
{
	AVLNODE *x = node;
	while (NULL != x)
	{
		traverseOpFn(x->key);
		StackPush(s, &(x->rc));
		x = x->lc;
	}
}

//Avl先序遍历（非递归）
void AvlTravPre(AVLTREE *avlTree, AvlTraverseOp *traverseOpFn)
{
	if (NULL == traverseOpFn || AvlEmpty(avlTree))
	{
		return ;
	}
	STACK avlNodeStack;
	StackNew(&avlNodeStack, sizeof(AVLNODE *), NULL);
	AVLNODE *node = avlTree->root;
	while (1)
	{
		visitAlongLeftBranch(node, &avlNodeStack, traverseOpFn);
		if (StackEmpty(&avlNodeStack))
		{
			break;
		}
		StackPop(&avlNodeStack, &node);
	}
	StackDispose(&avlNodeStack);
}

static void travPreRecAt(AVLNODE *node, AvlTraverseOp *traverseOpFn)
{
	if (NULL == node)
	{
		return ;
	}
	traverseOpFn(node->key);
	travPreRecAt(node->lc, traverseOpFn);
	travPreRecAt(node->rc, traverseOpFn);
}

//Avl先序遍历（递归）
void AvlTravPreRec(AVLTREE *avlTree, AvlTraverseOp *traverseOpFn)
{
	if (NULL == traverseOpFn || AvlEmpty(avlTree))
	{
		return ;
	}
	travPreRecAt(avlTree->root, traverseOpFn);
}

//二叉树中序遍历算法（迭代版#1）
static void travIn_V1(AVLTREE *avlTree, AvlTraverseOp *traverseOpFn)
{
	STACK avlNodeStack;
	//栈中存放avl节点指针
	StackNew(&avlNodeStack, sizeof(AVLNODE *), NULL);
	AVLNODE *node = avlTree->root;
	while (1)
	{
		//从当前节点出发，逐批入栈
		goAlongLeftBranch(node, &avlNodeStack);
		//所有节点处理完毕
		if (StackEmpty(&avlNodeStack))
		{
			break;
		}
		//弹出栈顶节点并访问之
		StackPop(&avlNodeStack, &node);
		traverseOpFn(node->key);
		node = node->rc; //转向右子树
	}
	StackDispose(&avlNodeStack);
}

//二叉树中序遍历算法（迭代版#2，版本#1的等价形式）
static void travIn_V2(AVLTREE *avlTree, AvlTraverseOp *traverseOpFn)
{
	STACK avlNodeStack;
	//栈中存放avl节点指针
	StackNew(&avlNodeStack, sizeof(AVLNODE *), NULL);
	AVLNODE *node = avlTree->root;
	while (1)
	{
		if (NULL != node)
		{
			StackPush(&avlNodeStack, &node);
			node = node->lc;
		}
		else if (!StackEmpty(&avlNodeStack))
		{
			StackPop(&avlNodeStack, &node);
			traverseOpFn(node->key);
			node = node->rc;
		}
		else
		{
			break;
		}
	}
	StackDispose(&avlNodeStack);
}

//定位节点node在中序遍历中的直接后继
static AVLNODE *succ(AVLNODE *node)
{
	AVLNODE *s = node;
	if (NULL != s->rc) //若有右孩子，则直接后继必在右子树中
	{
		s = s->rc;
		while (HasLChild(*s))
		{
			s = s->lc;
		}
	}
	else 
	{
		while (IsRChild(*s))
		{
			s = s->parent;
		}
		s = s->parent;
	}
	return s;
}

//二叉树中序遍历算法（迭代版#3）
static void travIn_V3(AVLTREE *avlTree, AvlTraverseOp *traverseOpFn)
{
	//前一步是否刚从右子树回溯--省去栈，仅O(1)辅助空间
	int backtrack = 0;
	AVLNODE *node = avlTree->root;
	while (1)
	{
		//若有左子树且不是刚刚回溯，则深入遍历左子树
		if (!backtrack && HasLChild(*node))
		{
			node = node->lc;
		} //否则无左子树或刚刚回溯
		else 
		{
			traverseOpFn(node->key);
			//右子树非空，深入右子树继续遍历，并关闭回溯标志
			if (HasRChild(*node))
			{
				node = node->rc;
				backtrack = 0;
			}
			else //右子树为空则回溯并设置回溯标志
			{
				node = succ(node);
				if (NULL == node)
				{
					break;
				}
				backtrack = 1;
			}
		}
	}
}

//Avl中序遍历（非递归）
void AvlTravIn(AVLTREE *avlTree, AvlTraverseOp *traverseOpFn)
{
	if (NULL == traverseOpFn || AvlEmpty(avlTree))
	{
		return ;
	}
	travIn_V1(avlTree, traverseOpFn);
}

static void travInRecAt(AVLNODE *node, AvlTraverseOp *traverseOpFn)
{
	if (NULL == node)
	{
		return ;
	}
	travInRecAt(node->lc, traverseOpFn);
	traverseOpFn(node->key);
	travInRecAt(node->rc, traverseOpFn);
}

//Avl中序遍历（递归）
void AvlTravInRec(AVLTREE *avlTree, AvlTraverseOp *traverseOpFn)
{
	if (NULL == traverseOpFn || AvlEmpty(avlTree))
	{
		return ;
	}
	travInRecAt(avlTree->root, traverseOpFn);
}

//在以s栈顶节点为根的子树中，
//找到最高左侧可见叶节点（highest leaf visible from left）
static void gotoHLVFL(STACK *s)
{
	AVLNODE *x;
	StackTop(s, &x);
	while (NULL !=x)
	{
		if (HasLChild(*x)) //尽可能向左
		{
			if (HasRChild(*x)) //若有右孩子，优先入栈
			{
				StackPush(s, &(x->rc));
			}
			StackPush(s, &(x->lc));
		}
		else
		{
			//此处x->rc可能为空，
			//左、右孩子都不存在时，将NULL入栈，便于while退出
			StackPush(s, &(x->rc));
		}
		StackTop(s, &x);
	}
	StackPop(s, NULL); //返回之前，弹出栈顶空节点
}

//Avl后序遍历（非递归）
void AvlTravPost(AVLTREE *avlTree, AvlTraverseOp *traverseOpFn)
{
	if (NULL == traverseOpFn || AvlEmpty(avlTree))
	{
		return ;
	}
	STACK avlNodeStack;
	StackNew(&avlNodeStack, sizeof(AVLNODE *), NULL);
	AVLNODE *x = avlTree->root;
	StackPush(&avlNodeStack, &x);
	while (!StackEmpty(&avlNodeStack))
	{
		AVLNODE *nodeTop;
		StackTop(&avlNodeStack, &nodeTop);
		//若栈顶非当前节点之父，则必为其右兄
		if (nodeTop != x->parent)
		{
			gotoHLVFL(&avlNodeStack);
		}
		StackPop(&avlNodeStack, &x);
		traverseOpFn(x->key);
	}
	StackDispose(&avlNodeStack);
}

static void travPostRecAt(AVLNODE *node, AvlTraverseOp *traverseOpFn)
{
	if (NULL == node)
	{
		return ;
	}
	travInRecAt(node->lc, traverseOpFn);
	travInRecAt(node->rc, traverseOpFn);
	traverseOpFn(node->key);
}

//Avl后序遍历（递归）
void AvlTravPostRec(AVLTREE *avlTree, AvlTraverseOp *traverseOpFn)
{	
	if (NULL == traverseOpFn || AvlEmpty(avlTree))
	{
		return ;
	}
	travPostRecAt(avlTree->root, traverseOpFn);
}

//Avl层序遍历
void AvlTravLevel(AVLTREE *avlTree, AvlTraverseOp *traverseOpFn)
{
	if (NULL == traverseOpFn || AvlEmpty(avlTree))
	{
		return ;
	}
	QUEUE avlNodeQueue;
	QueueNew(&avlNodeQueue, sizeof(AVLNODE *), NULL);
	AVLNODE *node = avlTree->root;
	QueueEn(&avlNodeQueue, &node);
	while (!QueueEmpty(&avlNodeQueue))
	{
		QueueDe(&avlNodeQueue, &node);
		traverseOpFn(node->key);
		if (HasLChild(*node))
		{
			QueueEn(&avlNodeQueue, &(node->lc));
		}
		if (HasRChild(*node))
		{
			QueueEn(&avlNodeQueue, &(node->rc));
		}
	}
	QueueDispose(&avlNodeQueue);
}

//Avl中查找关键码所在节点，hot指向当前节点的父节点
AVLNODE *AvlSearch(AVLTREE *avlTree, const void *e)
{
	AVLNODE *node = avlTree->root;
	avlTree->hot = NULL;
	while (NULL != node)
	{
		if (0 == avlTree->cmpFn(e, node->key))
		{
			break ;
		}
		avlTree->hot = node;
		if (0 < avlTree->cmpFn(e, node->key))
		{
			node = node->rc;
		}
		else
		{
			node = node->lc;
		}
	}
	return node;
}

int max(int a, int b)
{
	return (a > b ? a : b);
}

//更新节点node的高度
static int updateHeight(AVLNODE *node)
{
	return node->height = 1 + max(stature(node->lc), stature(node->rc));
}

//更新节点node及其祖先的高度
static void updateHeightAbove(AVLNODE *node)
{
	AVLNODE *cur = node;
	while (NULL != cur)
	{
		updateHeight(cur);
		cur = cur->parent;
	}
}

//按照“3+4”结构联接3个节点及四颗子树，返回重组后的局部子树根节点的位置（即b）
//子树根节点与上层节点之间的双向联接，均须由上层调用者完成
static AVLNODE *connect34(AVLNODE *a, AVLNODE *b, AVLNODE *c, AVLNODE *T0, AVLNODE *T1, AVLNODE *T2, AVLNODE *T3)
{
	a->lc = T0;
	if (T0)
	{
		T0->parent = a;
	}
	a->rc = T1;
	if (T1)
	{
		T1->parent = a;
	}
	updateHeight(a);
	
	c->lc = T2;
	if (T2)
	{
		T2->parent = c;
	}
	c->rc = T3;
	if (T3)
	{
		T3->parent = c;
	}
	updateHeight(c);
	
	b->lc = a;
	a->parent = b;
	b->rc = c;
	c->parent = b;
	updateHeight(b);
	return b;
}

//BST节点旋转变换统一算法（3节点+4子树），返回调整后局部子树根节点的位置
//注意：尽管子树根会正确指向上层节点（如果存在），但反向的联接须由上层函数完成
static AVLNODE *rotateAt(AVLNODE *grandsonNode)
{
	AVLNODE *p = grandsonNode->parent;
	AVLNODE *g = p->parent;
	if (IsLChild(*p)) //zig
	{
		if (IsLChild(*grandsonNode)) //zig-zig
		{
			p->parent = g->parent; //向上联接
			return connect34(grandsonNode, p, g, grandsonNode->lc, grandsonNode->rc, p->rc, g->rc);
		}
		else //zig-zag
		{
			grandsonNode->parent = g->parent; //向上联接
			return connect34(p, grandsonNode, g, p->lc, grandsonNode->lc, grandsonNode->rc, g->rc);
		}
	}
	else //zag
	{
		if (IsLChild(*grandsonNode)) //zag-zig
		{
			grandsonNode->parent = g->parent; //向上联接
			return connect34(g, grandsonNode, p, g->lc, grandsonNode->lc, grandsonNode->rc, p->rc);
		}
		else //zag-zag
		{
			p->parent = g->parent; //向上联接
			return connect34(g, p, grandsonNode, g->lc, p->lc, grandsonNode->lc, grandsonNode->rc);
		}
	}
}

//Avl中插入关键码
AVLNODE *AvlInsert(AVLTREE *avlTree, const void *e)
{
	AVLNODE *node = AvlSearch(avlTree, e);
	if (NULL != node)
	{
		return node;
	}
	//新节点初始化
	node = nodeNew(avlTree->keySize, e);
	if (NULL == node)
	{
		return NULL;
	}
	node->parent = avlTree->hot;
	//空树时指定根
	if (NULL == node->parent)
	{
		avlTree->root = node;
	}
	else
	{
		if (0 < avlTree->cmpFn(e, avlTree->hot->key))
		{
			avlTree->hot->rc = node;
		}
		else
		{
			avlTree->hot->lc = node;
		}
	}
	avlTree->size ++;
	
	AVLNODE *g = avlTree->hot;
	for (; g; g = g->parent)
	{
		if (!AvlBalanced(*g))
		{
			//重新接入原树
			if (IsRoot(*g)) 
			{
				avlTree->root = rotateAt(tallerChild(tallerChild(g)));
			}
			else if (IsLChild(*g))
			{
				AVLNODE *u = g->parent;
				u->lc = rotateAt(tallerChild(tallerChild(g)));
			}
			else
			{
				AVLNODE *u = g->parent;
				u->rc = rotateAt(tallerChild(tallerChild(g)));
			}
			break;
		}
		else
		{
			updateHeight(g);
		}
	}
	return node;
}

static void swap(void *dataAddr1, void *dataAddr2, int dataSize)
{
	char *tmp = (char *)malloc(dataSize);
	memcpy(tmp, dataAddr1, dataSize);
	memcpy(dataAddr1, dataAddr2, dataSize);
	memcpy(dataAddr2, tmp, dataSize);
	free(tmp);
}

//删除node所指节点
//返回值指向实际被删除节点的接替者，hot指向实际被删除节点的父亲，二者均可能是NULL
static AVLNODE *removeAt(AVLNODE **node, AVLNODE **hot, AvlFree *freeFn, int keySize)
{
	AVLNODE *w = *node; //实际被删除的节点
	AVLNODE *successor = NULL; //实际被删除节点的接替者
	
	//若**node的左子树为空，则直接将**node替换为其右子树
	if (!HasLChild(**node))
	{
		*node = (*node)->rc;
		successor = *node;
	}
	else if (!HasRChild(**node)) //右子树为空，对称处理
	{
		*node = (*node)->lc;
		successor = *node;
	}
	else //左、右子树均存在，则选择**node节点的直接后继作为实际被摘除节点
	{
		w = succ(w); //在右子树中找到直接后继
		swap((*node)->key, w->key, keySize);
		AVLNODE *u = w->parent;
		successor = w->rc; //此时待删除节点不可能有左孩子
		//隔离被删除节点w
		if (*node == u)
		{
			u->rc = successor;
		}
		else
		{
			u->lc = successor;
		}
	}
	//记录实际被删除节点的父亲
	*hot = w->parent;
	if (successor)
	{
		successor->parent = *hot; //将被删除节点的接替者与hot相联
	}
	nodeDispose(w, freeFn);
	return successor;
}

static int avlRemoveBase(AVLTREE *avlTree, void *e, AvlFree *freeFn)
{
	AVLNODE *node = AvlSearch(avlTree, e);
	//查不到要删除的节点，删除失败
	if (NULL == node)
	{
		return -1;
	}
	AVLNODE *hot = NULL; //记录删除节点的父亲
	removeAt(&node, &hot, freeFn, avlTree->keySize);
	avlTree->size --;
	
	AVLNODE *g = hot;
	for (; g; g = g->parent)
	{
		if (!AvlBalanced(*g))
		{
			AVLNODE *localRoot;
			if (IsRoot(*g))
			{
				localRoot = rotateAt(tallerChild(tallerChild(g)));
				avlTree->root = localRoot;
			}
			else if (IsLChild(*g))
			{
				AVLNODE *u = g->parent;
				localRoot = rotateAt(tallerChild(tallerChild(g)));
				u->lc = localRoot;
			}
			else
			{
				AVLNODE *u = g->parent;
				localRoot = rotateAt(tallerChild(tallerChild(g)));
				u->rc = localRoot;
			}
			g = localRoot;
		}
		updateHeight(g); //即便g未失衡，其高度也可能降低
	}
	return 0;
}

//Avl中删除关键码所在节点，返回值：0成功，!0失败
int AvlRemove(AVLTREE *avlTree, void *e)
{
	return avlRemoveBase(avlTree, e, avlTree->freeFn);
}

//Avl中删除关键码所在节点（无需深度删除关键码），返回值：0成功，!0失败
int AvlRemoveU(AVLTREE *avlTree, void *e)
{
	return avlRemoveBase(avlTree, e, NULL);
}