#ifndef C_DATASTRUCTURES_AVLTREE_H
#define C_DATASTRUCTURES_AVLTREE_H
#include "Utils.h"
#include <math.h>
#define COUNT 10

/** @struct AVLTNode
 *  @brief This structure describes n AVL Tree Node.
 *  @var AVLTNode::key
 *  Member 'key' is a pointer a pre-allocated object.
 *  @var AVLTNode::height
 *  Member 'height' holds the current height of the node.
 *  @var AVLTNode::left
 *  Member 'left' holds a reference to the left Node if present else it is Null.
 *  @var AVLTNode::right
 *  Member 'right' holds a reference to the right Node if present else it is Null.
 */
typedef struct AVLTNode{
    void * key;
    int height;
    struct AVLTNode *left;
    struct AVLTNode *right;
}AVLTNode;

/** @struct AVLTree
 *  @brief This structure implements a basic generic AVL Tree.
 *  @var AVLTree::root
 *  Member 'root' is a pointer to the root Node of the tree.
 *  @var AVLTree::nodeCount
 *  Member 'nodeCount' holds the number of nodes currently present in the tree.
 *  @var AVLTree::cmp
 *  Member 'cmp' is a pointer to the comparision function that is going to be used throughout the tree.
 */
typedef struct AVLTree{
    AVLTNode *root;
    uint32_t nodeCount;
    int16_t (*cmp)(const void*,const void*);
}AVLTree;


AVLTNode* AVLTreeDeleteNode(AVLTree *avlTree, AVLTNode* root, void *key);

int AVLTreeGetMinSteps(AVLTNode* node);

int AVLTreeGetMaxSteps(AVLTNode* node);

int AVLTreeGetSize(AVLTNode *root);

int AVLTreeIsPresent(AVLTree *avlTree, void* searchKey);

void AVLTreePrint(AVLTNode *root, void (printFn)(void *));

void AVLTreeInOrderPredecessor(AVLTree *avlTree, AVLTNode *root, AVLTNode *referenceNode, AVLTNode **rightMost);

void AVLTreeInOrderSuccessor(AVLTree *avlTree, AVLTNode* root, AVLTNode* referenceNode, AVLTNode **leftMost);

AVLTNode* AVLTreeInsert(AVLTree * avlTree, AVLTNode *node, void *key);

AVLTNode *AVLTreeSearch(AVLTree *avlTree, AVLTNode *node, char *key);

void **AVLTreeToArray(AVLTree *avlTree);

void AVLTreeFree(AVLTNode **node);

AVLTree* AVLTreeInitialize( int (*cmp)(const void*, const void*));

void AVLTreePrintStats(AVLTree *tree);

AVLTNode * AVLTreeMaxValueNode(AVLTNode* node);

AVLTNode * AVLTreeMinValueNode(AVLTNode* node);

void AVLTreePrintInOrder(AVLTNode* node, void (printFn)(void *));

AVLTNode * AVLTreeAddAll(AVLTree* avlTree,void **array, int length);

#endif //C_DATASTRUCTURES_AVLTREE_H
