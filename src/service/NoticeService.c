/*
 * 负责人：成员 B
 */
#include "NoticeService.h"

/*
 * 实现说明
 * --------
 * 本模块不自己管内存，只是把 Queue 包一层业务逻辑。
 *
 *     NoticeService  ┌────────────┐
 *                    │ pending ───┼──> Queue（ds/Queue.c）──> Notice 节点链
 *                    └────────────┘
 *
 * 通知用【队列】是天然合适的：
 *     新通知从队尾进（enqueue）
 *     发送时从队头出（dequeue）
 *     -> 先进先出，先到的通知先发出去
 *
 * 另外：notice_add / notice_sendNext 成功后会调用 history_record()
 * 写进操作历史。history_record 定义在 common.c，它通过全局
 * g_operationHistory 找到那个栈。所以这两个函数是跨模块的：
 *
 *     NoticeService.c ──调用──> common.c 的 history_record ──> Stack.c
 */

/* ------------------------------------------------------------------
 * 创建：内部建一个 Queue
 * ------------------------------------------------------------------ */
NoticeService* notice_service_create(void)
{
    NoticeService* s = (NoticeService*)malloc(sizeof(NoticeService));
    if (!s) {
        return NULL;
    }

    s->pending = queue_create();
    if (!s->pending) {                      /* Queue 建不出来就整个失败 */
        free(s);
        return NULL;
    }
    return s;
}

/* ------------------------------------------------------------------
 * 销毁：先放内部的 Queue（含所有节点），再放自己
 * ------------------------------------------------------------------ */
void notice_service_destroy(NoticeService* s)
{
    if (!s) {
        return;
    }
    if (s->pending) {
        queue_destroy(s->pending);
    }
    free(s);
}

/* ------------------------------------------------------------------
 * 功能 1：新增通知（入队）
 *
 * 注意 sent 字段被强制设成 false：刚进队列的通知当然还没发送。
 * 调用方即使是随便传的一个结构体，这里也会纠正过来。
 * ------------------------------------------------------------------ */
bool notice_add(NoticeService* s, Notice n)
{
    if (!s || !s->pending) {
        return false;
    }

    n.sent = false;                         /* 入队的通知一定还没发 */

    if (!enqueue(s->pending, n)) {
        return false;                       /* 节点分配失败 */
    }

    printf("通知已加入队列（当前队列长度：%d）\n", queue_size(s->pending));

    /* 写进操作历史。注意用的是【通知的接收者】作为 targetId */
    history_record("add", "Notice", n.target, "新增通知");
    return true;
}

/* ------------------------------------------------------------------
 * 功能 2：发送下一条通知（出队，FIFO 演示）
 *
 * 队列为空时返回 false，不崩溃。
 * ------------------------------------------------------------------ */
bool notice_sendNext(NoticeService* s)
{
    if (!s || !s->pending) {
        return false;
    }
    if (queue_isEmpty(s->pending)) {
        printf("队列为空，无通知可发送\n");
        return false;
    }

    Notice n = dequeue(s->pending);         /* 从队头取出一条 */
    n.sent = true;                          /* 标记为已发送 */

    printf("--- 发送下一条通知 ---\n[队列出队] 取出第 1 条\n");
    printf("[%s 已发送] -> %s\n", n.channel, n.target);
    printf("  标题：%s\n  内容：%s\n", n.title, n.content);
    printf("发送完成！（队列剩余 %d 条）\n", queue_size(s->pending));

    history_record("send", "Notice", n.target, "发送通知");
    return true;
}

/* ------------------------------------------------------------------
 * 功能 3：查看队首通知（不出队）
 * ------------------------------------------------------------------ */
void notice_peek(NoticeService* s)
{
    if (!s || !s->pending) {
        return;
    }
    if (queue_isEmpty(s->pending)) {
        printf("队列为空\n");
        return;
    }

    Notice n = queue_peek(s->pending);      /* 只读，不动队列 */
    printf("[队首预览] %s -> %s\n", n.channel, n.target);
    printf("   标题：%s\n   内容：%s\n", n.title, n.content);
    printf("（不会出队，仍在队列中）\n");
}

/* ------------------------------------------------------------------
 * 功能 4：列出所有待发通知
 *
 * 打印头部之后交给 queue_traverse 逐条输出，格式统一。
 * ------------------------------------------------------------------ */
void notice_listAll(NoticeService* s)
{
    if (!s || !s->pending) {
        return;
    }

    printf("--- 待发通知队列（共 %d 条）---\n", queue_size(s->pending));
    queue_traverse(s->pending);
}

/* ------------------------------------------------------------------
 * 功能 5：按通道过滤并打印（SMS / EMAIL / APP）
 *
 * 这里要遍历链表，所以要直接访问 pending->front 和节点的 next。
 * Queue.h 里 QueueNode 是公开的结构体定义，所以这样做是允许的。
 * 只读遍历，不修改队列。
 * ------------------------------------------------------------------ */
void notice_filterByChannel(NoticeService* s, const char* channel)
{
    if (!s || !s->pending || !channel) {
        return;
    }

    printf("--- 通道为 %s 的通知 ---\n", channel);

    QueueNode* cur = s->pending->front;
    int idx = 0;                            /* 命中的序号 */
    int hit = 0;                            /* 命中总数 */

    while (cur) {
        if (strcmp(cur->data.channel, channel) == 0) {
            printf("[%d] %-6s -> %-8s %s\n",
                   ++idx, cur->data.channel, cur->data.target, cur->data.title);
            hit++;
        }
        cur = cur->next;
    }

    printf("（共 %d 条）\n", hit);
}

/* ------------------------------------------------------------------
 * 功能 6：清空整个通知队列（需用户确认）
 *
 * 确认后才清空；取消则原样返回 false。
 * 注意 queue_clear 保留 Queue 结构体本身，所以清空后还能继续用。
 * ------------------------------------------------------------------ */
bool notice_clear(NoticeService* s)
{
    if (!s || !s->pending) {
        return false;
    }

    int n = queue_size(s->pending);         /* 先记住原来几条，用于提示 */

    printf("确认清空所有待发通知？(y/n): ");
    char confirm;
    if (scanf(" %c", &confirm) != 1) {
        return false;
    }
    if (confirm != 'y' && confirm != 'Y') {
        printf("已取消\n");
        return false;
    }

    queue_clear(s->pending);
    printf("队列已清空（原 %d 条通知全部丢弃）\n", n);
    return true;
}

/* ------------------------------------------------------------------
 * 通知管理菜单循环（主菜单 case 6 调用这里）
 * ------------------------------------------------------------------ */
void notice_service_menu(NoticeService* s)
{
    int choice;

    while (true) {
        printf("\n========== 通知管理 ==========\n");
        printf(" 1. 新增通知（入队）\n");
        printf(" 2. 发送下一条通知（出队）\n");
        printf(" 3. 查看队首通知\n");
        printf(" 4. 列出所有待发通知\n");
        printf(" 5. 按通道过滤（SMS/EMAIL/APP）\n");
        printf(" 6. 清空通知队列\n");
        printf(" 0. 返回主菜单\n");
        printf("==============================\n");
        printf("请输入选项: ");

        if (scanf("%d", &choice) != 1) {
            return;                         /* 输入结束，退回主菜单 */
        }

        switch (choice) {
            case 1: {
                Notice n;
                memset(&n, 0, sizeof(n));   /* 先清零，未填的字段都是空的 */

                printf("接收者 ID: ");
                if (scanf("%19s", n.target) != 1) return;

                printf("标题: ");
                if (scanf("%99s", n.title) != 1) return;

                printf("内容: ");
                if (scanf("%255s", n.content) != 1) return;

                printf("通道(1.SMS 2.EMAIL 3.APP): ");
                int ch;
                if (scanf("%d", &ch) != 1) return;

                /* 把 1/2/3 翻译成通道名。channel 只有 10 字节，"EMAIL" 放得下。 */
                if (ch == 1) {
                    strcpy(n.channel, "SMS");
                } else if (ch == 2) {
                    strcpy(n.channel, "EMAIL");
                } else {
                    strcpy(n.channel, "APP");
                }

                notice_add(s, n);
                break;
            }
            case 2:
                notice_sendNext(s);
                break;
            case 3:
                notice_peek(s);
                break;
            case 4:
                notice_listAll(s);
                break;
            case 5: {
                char ch[10];
                printf("请输入通道(SMS/EMAIL/APP): ");
                if (scanf("%9s", ch) != 1) return;
                notice_filterByChannel(s, ch);
                break;
            }
            case 6:
                notice_clear(s);
                break;
            case 0:
                return;
            default:
                printf("无效选项\n");
        }
    }
}
