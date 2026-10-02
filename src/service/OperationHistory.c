/*
 * 负责人：成员 B
 */
/* history_record 是 common 层跨服务回调，在 src/common.c 中实现（README §6.4.1），
 * 本文件不实现 */
#include "OperationHistory.h"

/* 全局 OperationHistory 在 main.c 定义，本文件 extern 引用 */
extern OperationHistory* g_operationHistory;

/*
 * 实现说明
 * --------
 * 本模块不自己管内存，只是把 Stack 包一层业务逻辑。
 *
 *     OperationHistory  ┌──────────┐
 *                       │ stack ───┼──> Stack（ds/Stack.c）──> Record[]
 *                       └──────────┘
 *
 * 全组的写入入口是 common.c 里的 history_record()，
 * 它通过 g_operationHistory->stack 调用 stack_push。
 * 所以这里只要保证 create 时把 stack 建好就行。
 *
 * 栈的语义正好适合操作历史：
 *     push = 记一次新操作
 *     pop  = 撤销最近一次操作（后进先出）
 */

/* ------------------------------------------------------------------
 * 创建：内部建一个 Stack
 * ------------------------------------------------------------------ */
OperationHistory* operation_history_create(void)
{
    OperationHistory* s = (OperationHistory*)malloc(sizeof(OperationHistory));
    if (!s) {
        return NULL;
    }

    s->stack = stack_create();
    if (!s->stack) {                        /* Stack 建不出来就整个失败 */
        free(s);
        return NULL;
    }
    return s;
}

/* ------------------------------------------------------------------
 * 销毁：先放内部的 Stack，再放自己
 * ------------------------------------------------------------------ */
void operation_history_destroy(OperationHistory* s)
{
    if (!s) {
        return;
    }
    if (s->stack) {
        stack_destroy(s->stack);
    }
    free(s);
}

/* ------------------------------------------------------------------
 * 功能 1：查看最近 n 次操作（栈顶 → 栈底）
 *
 * 做法：pop 出来打印，同时压进一个临时栈保存；
 *       打印完再把临时栈里的记录倒回原栈。
 *
 * 为什么要临时栈而不是直接 pop 掉：
 *     pop 会真的删除记录，而调用方只是想"看看"，历史不能丢。
 *
 * 两次"顺序反转"正好抵消，原栈会完全恢复：
 *     原栈 顶 A B C 底
 *     pop 进 tmp -> tmp 顶 C B A 底
 *     倒回原栈   -> 原栈 顶 A B C 底   ✓ 与原来一致
 * ------------------------------------------------------------------ */
void history_viewRecentN(OperationHistory* s, int n)
{
    if (!s || !s->stack) {
        return;
    }
    if (stack_isEmpty(s->stack)) {
        printf("暂无操作历史\n");
        return;
    }

    int total = stack_size(s->stack);
    int show  = (n < total) ? n : total;    /* n 超过总数就全部显示 */
    if (show < 0) {
        show = 0;
    }

    printf("--- 最近 %d 次操作（栈顶 → 栈底）---\n", show);

    Stack* tmp = stack_create();
    if (!tmp) {
        return;                             /* 建不出临时栈就放弃这次查看 */
    }

    for (int i = 0; i < show; i++) {
        Record r = stack_pop(s->stack);
        char buf[32];
        format_time(r.timestamp, buf);

        /* targetId 为空时显示 "-"（通知类操作没有编号） */
        const char* tid = (r.targetId[0] != '\0') ? r.targetId : "-";
        printf("[%d] %s  %-7s %-10s %-8s %s\n",
               i + 1, buf, r.opType, r.entityType, tid, r.description);

        stack_push(tmp, r);                 /* 存起来，一会儿还回去 */
    }

    /* 把临时栈里的记录倒回原栈 */
    while (!stack_isEmpty(tmp)) {
        stack_push(s->stack, stack_pop(tmp));
    }
    stack_destroy(tmp);
}

/* ------------------------------------------------------------------
 * 功能 2：撤销最近一次操作（栈弹出）
 *
 * 弹出栈顶并展示，用户可以选择继续撤销，所以递归调用自己。
 * 这里是真的把记录弹掉了 —— 撤销的语义就是"这次操作不算了"。
 * ------------------------------------------------------------------ */
void history_undoLast(OperationHistory* s)
{
    if (!s || !s->stack) {
        return;
    }
    if (stack_isEmpty(s->stack)) {
        printf("暂无操作可撤销\n");
        return;
    }

    Record r = stack_pop(s->stack);
    printf("--- 撤销最近一次操作 ---\n[栈弹出] 取出栈顶\n");
    printf("操作类型：%s\n", r.opType);
    printf("操作对象：%s %s\n", r.entityType, r.targetId);
    printf("操作描述：%s\n", r.description);

    printf("是否继续撤销？(y/n): ");
    char c;
    if (scanf(" %c", &c) != 1) {
        return;                             /* 输入结束（比如管道）直接返回 */
    }

    if (c == 'y' || c == 'Y') {
        history_undoLast(s);                /* 继续撤销下一条 */
    } else {
        printf("已记录撤销请求（请调用对应 Service 的恢复接口）\n");
    }
}

/* ------------------------------------------------------------------
 * 功能 3：清空整个历史栈（需用户确认）
 *
 * stack_clear 只把 top 归 -1，容量保留 —— 下次记录还能直接用这块内存。
 * ------------------------------------------------------------------ */
void history_clear(OperationHistory* s)
{
    if (!s || !s->stack) {
        return;
    }

    printf("确认清空所有历史记录？(y/n): ");
    char c;
    if (scanf(" %c", &c) != 1) {
        return;
    }
    if (c != 'y' && c != 'Y') {
        printf("已取消\n");
        return;
    }

    stack_clear(s->stack);
    printf("历史已清空\n");
}

/* ------------------------------------------------------------------
 * 操作历史菜单循环（主菜单 case 7 调用这里）
 * ------------------------------------------------------------------ */
void operation_history_menu(OperationHistory* s)
{
    int choice;

    while (true) {
        printf("\n========== 操作历史 ==========\n");
        printf(" 1. 查看最近 N 次操作\n");
        printf(" 2. 撤销最近一次操作\n");
        printf(" 3. 清空历史\n");
        printf(" 0. 返回主菜单\n");
        printf("==============================\n");
        printf("请输入选项: ");

        if (scanf("%d", &choice) != 1) {
            return;                         /* 输入结束，退回主菜单 */
        }

        switch (choice) {
            case 1: {
                int n;
                printf("请输入 N: ");
                if (scanf("%d", &n) != 1) return;
                history_viewRecentN(s, n);
                break;
            }
            case 2:
                history_undoLast(s);
                break;
            case 3:
                history_clear(s);
                break;
            case 0:
                return;
            default:
                printf("无效选项\n");
        }
    }
}
