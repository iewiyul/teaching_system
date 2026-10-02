/*
 * 顺序栈：存储 Record
 * 负责人：成员 B
 *
 * 实现说明
 * --------
 * 底层是一块【连续数组】（堆上 malloc 出来的），用 top 记录栈顶下标。
 *
 *     data ──> [ Record ][ Record ][ Record ][     ]
 *                  0         1         2       3
 *                ↑
 *               top = 1  （装了 2 个，下一个空位是下标 2）
 *
 * 三个关键数字：
 *     top == -1                     空栈
 *     元素个数       = top + 1
 *     满的条件       = (top + 1 == capacity)
 *
 * 扩容策略：容量不足时扩到 2 倍，保证均摊 O(1)。
 * 空栈时 pop/peek 返回"全零 Record"，绝不崩溃。
 */
#include "Stack.h"

#include <stdlib.h>
#include <string.h>

/* 第一次扩容的初始容量。必须是 > 0 的值，否则 0*2 永远是 0，扩不动。 */
#define STACK_INIT_CAPACITY 4

/* ------------------------------------------------------------------
 * grow —— 把内部数组的容量扩到 newCap 个元素
 *
 * 这是本文件唯一的私有帮手函数，所以加 static：
 * 只有 Stack.c 内部能调用，别的 .c 文件看不见它。
 *
 * 【最容易写错的一行】realloc 的结果必须先存进临时变量。
 *     ❌ s->data = realloc(s->data, ...);
 *        失败时返回 NULL，s->data 变成 NULL，
 *        原来那块内存的地址就丢了 -> 内存泄漏 + 栈彻底损坏
 *     ✅ 先存 p，确认成功了再赋给 s->data
 * ------------------------------------------------------------------ */
static bool grow(Stack* s, int newCap)
{
    if (!s || newCap <= s->capacity) {
        return true;                        /* 不用扩 */
    }

    /* realloc 的单位是【字节】，所以要 元素大小 × 个数 */
    Record* p = (Record*)realloc(s->data, sizeof(Record) * (size_t)newCap);
    if (!p) {
        return false;                       /* 失败：原内存还在，s->data 依然有效 */
    }

    s->data     = p;
    s->capacity = newCap;
    return true;
}

/* ------------------------------------------------------------------
 * 创建空栈
 * ------------------------------------------------------------------ */
Stack* stack_create(void)
{
    Stack* s = (Stack*)malloc(sizeof(Stack));
    if (!s) {
        return NULL;                        /* 申请失败，交回 NULL 让调用方判断 */
    }

    s->data     = NULL;                     /* 数组还没申请，第一次 push 时才建 */
    s->top      = -1;                       /* -1 表示空栈 */
    s->capacity = 0;
    return s;
}

/* ------------------------------------------------------------------
 * 销毁：把申请过的内存还回去
 *
 * 顺序不能反：先 free 里面的数组，再 free 结构体本身。
 * 反过来的话，s->data 这个信息就没了，那块数组永远找不回来。
 * ------------------------------------------------------------------ */
void stack_destroy(Stack* s)
{
    if (!s) {
        return;
    }
    free(s->data);                          /* free(NULL) 是安全的 */
    free(s);
}

/* ------------------------------------------------------------------
 * 压栈：把一条记录放到栈顶
 *
 * 顺序不能反：先 top++，再写入。
 *   空栈时 top == -1，如果先写就是 data[-1] —— 越界写，会踩坏别的内存。
 *   先 top++ 把 top 挪到合法的 0，再写就没问题。
 * ------------------------------------------------------------------ */
bool stack_push(Stack* s, Record r)
{
    if (!s) {
        return false;
    }

    /* 满了？（元素个数 == 容量） */
    if (s->top + 1 >= s->capacity) {
        /* capacity 为 0 时 2 倍还是 0，必须特判成初始容量 */
        int newCap = (s->capacity == 0) ? STACK_INIT_CAPACITY : s->capacity * 2;
        if (!grow(s, newCap)) {
            return false;                   /* 扩容失败，压栈也失败 */
        }
    }

    s->data[++s->top] = r;                  /* 先自增到下一个位置，再写入 */
    return true;
}

/* ------------------------------------------------------------------
 * 出栈：拿走栈顶那条记录
 *
 * 空栈时返回"全零 Record"而不是崩溃 —— 这是本项目 Stack.h 定下的约定。
 * 顺序和 push 相反：先取值，再 top--。
 * ------------------------------------------------------------------ */
Record stack_pop(Stack* s)
{
    Record zero;
    memset(&zero, 0, sizeof(zero));         /* 先准备好"空记录" */

    if (!s || s->top < 0) {
        return zero;                        /* 空栈：返回全零 */
    }

    Record r = s->data[s->top];             /* 先取出来 */
    s->top--;                               /* 再移动栈顶 */
    return r;
}

/* ------------------------------------------------------------------
 * 查看栈顶，但不出栈
 * ------------------------------------------------------------------ */
Record stack_peek(Stack* s)
{
    Record zero;
    memset(&zero, 0, sizeof(zero));

    if (!s || s->top < 0) {
        return zero;
    }
    return s->data[s->top];                 /* 只读，不动 top */
}

/* ------------------------------------------------------------------
 * 三个小工具
 * ------------------------------------------------------------------ */

bool stack_isEmpty(Stack* s)
{
    return !s || s->top == -1;
}

int stack_size(Stack* s)
{
    return !s ? 0 : s->top + 1;
}

/* 清空栈：top 归 -1，但【容量保留】。
 * 保留容量的好处：再来数据时不用重新扩容。 */
void stack_clear(Stack* s)
{
    if (s) {
        s->top = -1;
    }
}
