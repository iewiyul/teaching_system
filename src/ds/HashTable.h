/*
链地址法哈希表：key=char[]（学号），value=Student*
负责人：成员 D
*/
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "../common.h"
#include "../entity/Student.h"

typedef struct HashNode {
    char           key[MAX_ID_LEN];
    Student*       value;          /* 哈希表只存指针，不复制 Student */
    struct HashNode* next;
} HashNode;

typedef struct {
    HashNode** buckets;
    int        size;
    int        capacity;
} HashTable;

/* 创建指定容量的 HashTable；桶数组预分配 */
HashTable* hashtable_create(int capacity);
/* 销毁并释放所有桶节点（不释放 Student 指针指向的对象） */
void       hashtable_destroy(HashTable* ht);
/* 按 key 插入；key 已存在则覆盖 value；满载自动扩容 */
bool       hashtable_insert(HashTable* ht, const char* key, Student* s);
/* 按 key 删除首个匹配节点 */
bool       hashtable_remove(HashTable* ht, const char* key);
/* 按 key 查找；返回内部 value 指针（不要 free） */
Student*   hashtable_find(HashTable* ht, const char* key);
/* 按 key 判断是否存在 */
bool       hashtable_contains(HashTable* ht, const char* key);
/* 当前元素个数 */
int        hashtable_size(HashTable* ht);

/* 最长链长度（验收 §9.4 功能 6 用） */
int        hashtable_getLongestChain(HashTable* ht);
/* 当前装填因子 = size / capacity */
double     hashtable_getLoadFactor(HashTable* ht);
/* 输出容量/已存/装填因子/最长链/桶分布图 */
void       hashtable_printStats(HashTable* ht);

#endif /* HASHTABLE_H */