/*
 * 负责人：成员 B
 */
/* history_record 是 common 层跨服务回调，在 src/common.c 中实现（README §6.4.1），
 * 本文件不实现 */
#include "OperationHistory.h"

/* 全局 OperationHistory 在 main.c 定义，本文件 extern 引用 */
extern OperationHistory* g_operationHistory;

OperationHistory* operation_history_create(void)             { /* TODO(组员): 实现 */ return NULL; }
void              operation_history_destroy(OperationHistory* s) { /* TODO(组员): 实现 */ (void)s; }

void   history_viewRecentN(OperationHistory* s, int n) { /* TODO(组员): 实现 */ (void)s; (void)n; }
void   history_undoLast(OperationHistory* s)           { /* TODO(组员): 实现 */ (void)s; }
void   history_clear(OperationHistory* s)              { /* TODO(组员): 实现 */ (void)s; }

void   operation_history_menu(OperationHistory* s) {
    /* TODO(组员): 实现菜单循环（README §7.6.2） */
    (void)s;
}
