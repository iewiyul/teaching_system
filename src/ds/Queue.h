/*
链式队列（FIFO）：存储 Notice
负责人：成员 B
*/
#ifndef QUEUE_H
#define QUEUE_H

#include "../common.h"
#include "../entity/Notice.h"

typedef struct QueueNode {
    Notice           data;
    struct QueueNode* next;
} QueueNode;

typedef struct {
    QueueNode* front;
    QueueNode* rear;
    int        size;
} Queue;

/* 创建空 Queue；front=rear=NULL */
Queue*  queue_create(void);
/* 销毁并释放所有节点 */
void    queue_destroy(Queue* q);
/* 队尾入队；失败返回 false */
bool    enqueue(Queue* q, Notice n);
/* 队头出队；空队列返回全零 Notice */
Notice  dequeue(Queue* q);
/* 查看队头；空队列返回全零 Notice */
Notice  queue_peek(Queue* q);

/* 是否为空（front == NULL） */
bool    queue_isEmpty(Queue* q);
/* 当前元素个数 */
int     queue_size(Queue* q);
/* 按 [N] channel → target title 格式遍历打印 */
void    queue_traverse(Queue* q);
/* 释放所有节点，size=0（Queue 结构本身保留） */
void    queue_clear(Queue* q);

#endif /* QUEUE_H */