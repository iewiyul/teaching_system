/*
堆：存储 Student，通过 isMinHeap 切换小/大顶堆
负责人：成员 E
*/
#ifndef HEAP_H
#define HEAP_H

#include "../common.h"
#include "../entity/Student.h"

typedef struct {
    Student* data;
    int      size;
    int      capacity;
    bool     isMinHeap;   /* true=小顶堆（升序），false=大顶堆（降序） */
} Heap;

/* 创建空 Heap；isMinHeap=true 为小顶堆，false 为大顶堆 */
Heap*    heap_create(bool isMinHeap);
/* 销毁并释放内部数组 */
void     heap_destroy(Heap* h);
/* 插入 Student；容量不足自动 2 倍扩容 */
bool     heap_insert(Heap* h, Student s);
/* 弹出堆顶并维护堆性质；空堆返回全零 Student */
Student  heap_extractTop(Heap* h);
/* 查看堆顶；空堆返回全零 Student */
Student  heap_peek(Heap* h);

/* 当前元素个数 */
int      heap_size(Heap* h);
/* 是否为空 */
bool     heap_isEmpty(Heap* h);

/* 原地堆排序 arr[0..n)；isMinHeap=true 升序，false 降序 */
void     heap_sort(Student* arr, int n, bool isMinHeap);
/* 从 all[0..n) 中取前 k 大写入 result（k <= n） */
void     heap_topK_max(Student* all, int n, int k, Student* result);

#endif /* HEAP_H */