/*
二叉排序树：key=double（GPA）或 int（badLevel），value=Student
负责人：成员 D
*/
#ifndef BST_H
#define BST_H

#include "../common.h"
#include "../entity/Student.h"

typedef struct BSTNode {
    double          key;
    Student         value;
    struct BSTNode* left;
    struct BSTNode* right;
} BSTNode;

typedef struct {
    BSTNode* root;
    int      size;
} BST;

/* 创建空 BST */
BST*      bst_create(void);
/* 销毁并释放所有节点 */
void      bst_destroy(BST* tree);
/* 按 key 插入；key 已存在则覆盖 value */
bool      bst_insert(BST* tree, double key, Student value);
/* 按 key 删除节点；不存在返回 false */
bool      bst_remove(BST* tree, double key);
/* 按 key 查找；返回指向内部 value 的指针（不要 free） */
Student*  bst_find(BST* tree, double key);

/* 区间查询 [lowKey, highKey]；命中写入 result，*count 为命中数 */
void      bst_rangeQuery(BST* tree, double lowKey, double highKey,
                         Student* result, int* count);
/* 中序遍历；结果按 key 升序写入 result，*count 为节点数 */
void      bst_inorder(BST* tree, Student* result, int* count);

/* 当前节点个数 */
int       bst_size(BST* tree);
/* 清空树（root=NULL，size=0） */
void      bst_clear(BST* tree);

#endif /* BST_H */