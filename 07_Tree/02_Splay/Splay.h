/* Splay.h */
#ifndef _SPLAY_H
#define _SPLAY_H

//0--代表比较相同，>0--代表dataAddr<keyAddr，<0--代表dataAddr>keyAddr
typedef int  SplayCmp(const void *keyAddr, const void *dataAddr);
typedef void SplayFree(void *);
typedef void SplayTraverseOp(void *);

typedef struct splay_node
{
	struct splay_node *parent;
	struct splay_node *lc;
	struct splay_node *rc;
	char key[0];
}SPLAYNODE;

typedef struct
{
	SPLAYNODE *root;
	SPLAYNODE *hot; //“命中”节点的父亲
	int size;
	int keySize;
	SplayCmp *cmpFn;
	SplayFree *freeFn;
}SPLAYTREE;

//Splay初始化
void SplayNew(SPLAYTREE *splay, int keySize, SplayCmp *cmpFn, SplayFree *freeFn);
//Splay销毁
void SplayDispose(SPLAYTREE *splay);
//Splay判空
int SplayEmpty(SPLAYTREE *splay);
//Splay规模
int SplaySize(SPLAYTREE *splay);
//Splay中序遍历（非递归）
void SplayTravIn(SPLAYTREE *splay, SplayTraverseOp *traverseOpFn);
//Splay中序遍历（递归）
void SplayTravInRec(SPLAYTREE *splay, SplayTraverseOp *traverseOpFn);
//Splay中查找关键码
SPLAYNODE *SplaySearch(SPLAYTREE *splay, const void *e);
//Splay判断某关键码是否在节点中，返回值：0--不在，!0--存在
int SplayFind(SPLAYTREE *splay, SPLAYNODE *node, const void *e);
//Splay中插入关键码
SPLAYNODE *SplayInsert(SPLAYTREE *splay, const void *e);
//Splay中删除关键码，返回值：0--成功，!0--失败
int SplayRemove(SPLAYTREE *splay, void *e);
//Splay中删除关键码（关键码非深度删除），返回值：0--成功，!0--失败
int SplayRemoveU(SPLAYTREE *splay, void *e);
#endif