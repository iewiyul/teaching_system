/*
负责人：成员 D
*/
#include "BST.h"

BST*     bst_create(void)                { /* TODO(组员): 实现 */ return NULL; }
void     bst_destroy(BST* tree)          { /* TODO(组员): 实现 */ (void)tree; }

bool     bst_insert(BST* tree, double key, Student value) { /* TODO(组员): 实现 */ (void)tree; (void)key; (void)value; return false; }
bool     bst_remove(BST* tree, double key)                { /* TODO(组员): 实现 */ (void)tree; (void)key; return false; }
Student* bst_find(BST* tree, double key)                  { /* TODO(组员): 实现 */ (void)tree; (void)key; return NULL; }

void     bst_rangeQuery(BST* tree, double lowKey, double highKey,
                        Student* result, int* count)      { /* TODO(组员): 实现 */ (void)tree; (void)lowKey; (void)highKey; (void)result; (void)count; }
void     bst_inorder(BST* tree, Student* result, int* count) { /* TODO(组员): 实现 */ (void)tree; (void)result; (void)count; }

int      bst_size(BST* tree)           { /* TODO(组员): 实现 */ (void)tree; return 0; }
void     bst_clear(BST* tree)          { /* TODO(组员): 实现 */ (void)tree; }
