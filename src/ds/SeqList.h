/*
顺序表：存储 Teacher
负责人：成员 C
*/
#ifndef SEQLIST_H
#define SEQLIST_H

#include "../common.h"
#include "../entity/Teacher.h"

typedef struct {
    Teacher* data;
    int      size;
    int      capacity;
} SeqList;

/* 创建空 SeqList；data=NULL, capacity=0 */
SeqList*  seqlist_create(void);
/* 销毁并释放内部 Teacher 数组（不释放 Teacher 指向的对象） */
void      seqlist_destroy(SeqList* s);
/* 末尾追加 Teacher；容量不足自动 2 倍扩容 */
bool      seqlist_insert(SeqList* s, Teacher t);
/* 按工号删除首个匹配项；后续元素前移 */
bool      seqlist_remove(SeqList* s, const char* id);
/* 按工号替换为新 Teacher */
bool      seqlist_update(SeqList* s, const char* id, Teacher t);
/* 按工号查找；返回内部指针（不要 free） */
Teacher*  seqlist_find(SeqList* s, const char* id);
/* 按下标取 Teacher 指针；越界返回 NULL */
Teacher*  seqlist_at(SeqList* s, int index);

/* 原地按工号升序重排 */
void      seqlist_sort_by_id(SeqList* s);
/* 原地按授课评分降序重排 */
void      seqlist_sort_by_rating_desc(SeqList* s);

/* 按表格格式打印全部 Teacher */
void      seqlist_traverse(SeqList* s);
/* 当前元素个数 */
int       seqlist_size(SeqList* s);
/* 是否为空 */
bool      seqlist_isEmpty(SeqList* s);

#endif /* SEQLIST_H */