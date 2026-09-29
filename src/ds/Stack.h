/*
顺序栈：存储 Record
负责人：成员 B
*/
#ifndef STACK_H
#define STACK_H

#include "../common.h"
#include "../entity/Record.h"

typedef struct {
    Record* data;
    int     top;        /* -1 表示空栈 */
    int     capacity;
} Stack;

/* 创建空 Stack；top=-1, capacity=0 */
Stack*  stack_create(void);
/* 销毁并释放内部数组 */
void    stack_destroy(Stack* s);
/* 压栈；容量不足自动 2 倍扩容 */
bool    stack_push(Stack* s, Record r);
/* 出栈；空栈返回全零 Record */
Record  stack_pop(Stack* s);
/* 查看栈顶；空栈返回全零 Record */
Record  stack_peek(Stack* s);

/* 是否为空（top == -1） */
bool    stack_isEmpty(Stack* s);
/* 当前元素个数（top + 1） */
int     stack_size(Stack* s);
/* 清空栈：top=-1，capacity 保留 */
void    stack_clear(Stack* s);

#endif /* STACK_H */