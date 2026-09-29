/*
负责人：成员 E
*/
#include "Heap.h"

Heap*    heap_create(bool isMinHeap)              { /* TODO(组员): 实现 */ (void)isMinHeap; return NULL; }
void     heap_destroy(Heap* h)                    { /* TODO(组员): 实现 */ (void)h; }

bool     heap_insert(Heap* h, Student s)          { /* TODO(组员): 实现 */ (void)h; (void)s; return false; }
Student  heap_extractTop(Heap* h)                 { /* TODO(组员): 实现 */ (void)h; Student z = {0}; return z; }
Student  heap_peek(Heap* h)                       { /* TODO(组员): 实现 */ (void)h; Student z = {0}; return z; }

int      heap_size(Heap* h)                       { /* TODO(组员): 实现 */ (void)h; return 0; }
bool     heap_isEmpty(Heap* h)                    { /* TODO(组员): 实现 */ (void)h; return true; }

void     heap_sort(Student* arr, int n, bool isMinHeap)               { /* TODO(组员): 实现 */ (void)arr; (void)n; (void)isMinHeap; }
void     heap_topK_max(Student* all, int n, int k, Student* result)   { /* TODO(组员): 实现 */ (void)all; (void)n; (void)k; (void)result; }
