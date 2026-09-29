/*
单向链表：存储 Student
负责人：成员 C
*/
#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "../common.h"
#include "../entity/Student.h"

typedef struct ListNode {
    Student          data;
    struct ListNode* next;
} ListNode;

typedef struct {
    ListNode* head;
    int       size;
} LinkedList;

/* 创建空 LinkedList */
LinkedList* linkedlist_create(void);
/* 销毁并释放所有节点 */
void        linkedlist_destroy(LinkedList* l);
/* 头插：在链表头部插入 Student */
bool        linkedlist_insert(LinkedList* l, Student s);
/* 尾插：在链表尾部插入 Student */
bool        linkedlist_insertTail(LinkedList* l, Student s);
/* 按学号删除首个匹配节点 */
bool        linkedlist_remove(LinkedList* l, const char* id);
/* 按学号查找；返回内部指针（不要 free） */
Student*    linkedlist_find(LinkedList* l, const char* id);

/* 原地按绩点降序重排 */
void        linkedlist_sort_by_gpa_desc(LinkedList* l);

/* 按 [N] 学号 姓名 格式遍历打印 */
void        linkedlist_traverse(LinkedList* l);
/* 当前节点个数 */
int         linkedlist_size(LinkedList* l);
/* 是否为空 */
bool        linkedlist_isEmpty(LinkedList* l);

#endif /* LINKEDLIST_H */