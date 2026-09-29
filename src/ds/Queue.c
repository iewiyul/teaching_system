/*
负责人：成员 B
*/
#include "Queue.h"

Queue*  queue_create(void)            { /* TODO(组员): 实现 */ return NULL; }
void    queue_destroy(Queue* q)      { /* TODO(组员): 实现 */ (void)q; }

bool    enqueue(Queue* q, Notice n)   { /* TODO(组员): 实现 */ (void)q; (void)n; return false; }
Notice  dequeue(Queue* q)            { /* TODO(组员): 实现 */ (void)q; Notice z = {0}; return z; }
Notice  queue_peek(Queue* q)         { /* TODO(组员): 实现 */ (void)q; Notice z = {0}; return z; }

bool    queue_isEmpty(Queue* q)      { /* TODO(组员): 实现 */ (void)q; return true; }
int     queue_size(Queue* q)         { /* TODO(组员): 实现 */ (void)q; return 0; }
void    queue_traverse(Queue* q)     { /* TODO(组员): 实现 */ (void)q; }
void    queue_clear(Queue* q)        { /* TODO(组员): 实现 */ (void)q; }
