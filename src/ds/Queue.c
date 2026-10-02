/*
 * 链式队列（FIFO）：存储 Notice
 * 负责人：成员 B
 *
 * 实现说明
 * --------
 * 底层是【单向链表】，每个节点里有一个 next 指针指向下一个节点。
 *
 *     front ──> [ Notice | next ] ──> [ Notice | next ] ──> [ Notice | next=NULL ]
 *                  ↑                                            ↑
 *                出队从这头摘                                  入队挂到这头
 *                              rear ────────────────────────────┘
 *
 * 和 Stack 的根本区别：
 *     Stack  用一块连续数组，元素"紧挨着"，下标直接算位置
 *     Queue  用一个个分散的节点，靠 next 指针串起来，只能顺着走
 *
 * 两个最容易出错的地方：
 *     1. 第一个节点入队时，front 和 rear 都要指向它
 *     2. 出队后如果队列空了，rear 必须置 NULL（否则 rear 是野指针）
 */
#include "Queue.h"

#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------
 * 创建空队列
 * ------------------------------------------------------------------ */
Queue* queue_create(void)
{
    Queue* q = (Queue*)malloc(sizeof(Queue));
    if (!q) {
        return NULL;
    }

    q->front = NULL;
    q->rear  = NULL;
    q->size  = 0;
    return q;
}

/* ------------------------------------------------------------------
 * 清空队列：释放所有节点，但 Queue 结构体本身保留
 *
 * 释放链表的标准写法：先存下一个节点的地址，再 free 当前节点。
 * 顺序反了就变成 use-after-free（读已释放内存）。
 * ------------------------------------------------------------------ */
void queue_clear(Queue* q)
{
    if (!q) {
        return;
    }

    QueueNode* p = q->front;
    while (p) {
        QueueNode* next = p->next;          /* 先记住下一个 */
        free(p);                            /* 再释放当前 */
        p = next;
    }

    q->front = NULL;
    q->rear  = NULL;
    q->size  = 0;
}

/* ------------------------------------------------------------------
 * 销毁队列：清空所有节点，再释放 Queue 本身
 * ------------------------------------------------------------------ */
void queue_destroy(Queue* q)
{
    if (!q) {
        return;
    }
    queue_clear(q);                         /* 先放掉所有节点 */
    free(q);                                /* 再放掉队列结构体 */
}

/* ------------------------------------------------------------------
 * 入队：新节点挂到队尾
 *
 * 三种情况都要处理对：
 *     空队列   front 和 rear 同时指向新节点（新节点既是头也是尾）
 *     非空     rear->next 指向新节点，然后 rear 前移
 *     失败     malloc 返回 NULL，原队列不动
 * ------------------------------------------------------------------ */
bool enqueue(Queue* q, Notice n)
{
    if (!q) {
        return false;
    }

    QueueNode* node = (QueueNode*)malloc(sizeof(QueueNode));
    if (!node) {
        return false;                       /* 分配失败 */
    }

    node->data = n;
    node->next = NULL;                      /* 新节点一定是最后一个 */

    if (q->rear) {
        q->rear->next = node;               /* 老队尾连到新节点 */
    } else {
        q->front = node;                    /* 空队列：新节点也是队头 */
    }

    q->rear = node;                         /* 队尾前移 */
    q->size++;
    return true;
}

/* ------------------------------------------------------------------
 * 出队：从队头摘掉一个节点
 *
 * 【关键】出队后如果队列空了，必须把 rear 也置 NULL。
 * 否则 rear 还指着已经被 free 的节点，下次入队写 rear->next
 * 就是"写已释放内存"，可能立刻崩，也可能在别处莫名其妙地炸。
 * ------------------------------------------------------------------ */
Notice dequeue(Queue* q)
{
    Notice zero;
    memset(&zero, 0, sizeof(zero));         /* 先准备好"空通知" */

    if (!q || !q->front) {
        return zero;                        /* 空队列：返回全零 */
    }

    QueueNode* node = q->front;             /* 要摘掉的节点 */
    Notice n = node->data;                  /* 先把数据拿出来 */

    q->front = node->next;                  /* 队头后移 */
    if (!q->front) {
        q->rear = NULL;                     /* 【关键】空了就把 rear 也清掉 */
    }

    free(node);
    q->size--;
    return n;
}

/* ------------------------------------------------------------------
 * 查看队头，但不出队
 * ------------------------------------------------------------------ */
Notice queue_peek(Queue* q)
{
    Notice zero;
    memset(&zero, 0, sizeof(zero));

    if (!q || !q->front) {
        return zero;
    }
    return q->front->data;                  /* 只读，不动 front */
}

/* ------------------------------------------------------------------
 * 两个小工具
 * ------------------------------------------------------------------ */

bool queue_isEmpty(Queue* q)
{
    return !q || q->front == NULL;
}

int queue_size(Queue* q)
{
    return !q ? 0 : q->size;
}

/* ------------------------------------------------------------------
 * 遍历打印全部待发通知
 *
 * 输出格式（NoticeService.c 会直接调用它）：
 *     [1] SMS    → T102      关于下周三教研会议
 * ------------------------------------------------------------------ */
void queue_traverse(Queue* q)
{
    if (!q) {
        return;
    }

    int i = 1;
    QueueNode* p = q->front;
    while (p) {
        printf("[%d] %-6s \xe2\x86\x92 %-8s %s\n",
               i, p->data.channel, p->data.target, p->data.title);
        p = p->next;
        i++;
    }
}
