/*
操作历史：底层用 Stack 存 Record；提供 history_record 跨服务回调
负责人：成员 B
*/
#ifndef OPERATION_HISTORY_H
#define OPERATION_HISTORY_H

#include "../common.h"
#include "../entity/Record.h"
#include "../ds/Stack.h"

typedef struct OperationHistory {
    Stack* stack;
} OperationHistory;

/* 创建 OperationHistory；内部 new 一个 Stack 存 Record */
OperationHistory* operation_history_create(void);
/* 销毁并释放内部 Stack */
void              operation_history_destroy(OperationHistory* s);

/* 查看最近 n 次操作（栈顶 → 栈底，临时栈 pop 后再 push 回去） */
void   history_viewRecentN(OperationHistory* s, int n);
/* 撤销最近一次操作（栈顶弹出并展示，可连续撤销） */
void   history_undoLast(OperationHistory* s);
/* 清空整个历史栈；需用户 y 确认 */
void   history_clear(OperationHistory* s);

/* 操作历史菜单循环；B 在 OperationHistory.c 末尾实现 */
void   operation_history_menu(OperationHistory* s);

#endif /* OPERATION_HISTORY_H */
