/*
链地址法哈希表：key=char[]（工号），value=Teacher*
负责人：成员 D
*/
#ifndef TEACHER_HASHTABLE_H
#define TEACHER_HASHTABLE_H

#include "../common.h"
#include "../entity/Teacher.h"

typedef struct TeacherHashNode {
    char                key[MAX_ID_LEN];
    Teacher*            value;
    struct TeacherHashNode* next;
} TeacherHashNode;

typedef struct {
    TeacherHashNode** buckets;
    int               size;
    int               capacity;
} TeacherHashTable;

/* 创建指定容量的 TeacherHashTable */
TeacherHashTable* teacher_hashtable_create(int capacity);
/* 销毁并释放所有桶节点（不释放 Teacher 指针指向的对象） */
void              teacher_hashtable_destroy(TeacherHashTable* ht);
/* 按 key 插入；key 已存在则覆盖 value；满载自动扩容 */
bool              teacher_hashtable_insert(TeacherHashTable* ht, const char* key, Teacher* t);
/* 按 key 删除首个匹配节点 */
bool              teacher_hashtable_remove(TeacherHashTable* ht, const char* key);
/* 按 key 查找；返回内部 value 指针（不要 free） */
Teacher*          teacher_hashtable_find(TeacherHashTable* ht, const char* key);
/* 按 key 判断是否存在 */
bool              teacher_hashtable_contains(TeacherHashTable* ht, const char* key);
/* 当前元素个数 */
int               teacher_hashtable_size(TeacherHashTable* ht);

#endif /* TEACHER_HASHTABLE_H */