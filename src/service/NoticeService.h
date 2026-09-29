/*
通知业务：底层用 Queue 存 Notice（FIFO）
负责人：成员 B
*/
#ifndef NOTICE_SERVICE_H
#define NOTICE_SERVICE_H

#include "../common.h"
#include "../entity/Notice.h"
#include "../ds/Queue.h"

typedef struct NoticeService {
    Queue* pending;
} NoticeService;

/* 创建 NoticeService；内部 new 一个 Queue 存 Notice */
NoticeService* notice_service_create(void);
/* 销毁并释放内部 Queue */
void           notice_service_destroy(NoticeService* s);

/* 新增通知（入队）；成功后 history_record("add", "Notice", n.target) */
bool   notice_add(NoticeService* s, Notice n);
/* 发送队首通知（出队，FIFO）；成功后 history_record("send", "Notice", n.target) */
bool   notice_sendNext(NoticeService* s);
/* 查看队首通知（不出队） */
void   notice_peek(NoticeService* s);
/* 遍历打印全部待发通知 */
void   notice_listAll(NoticeService* s);
/* 按通道过滤并打印（SMS / EMAIL / APP） */
void   notice_filterByChannel(NoticeService* s, const char* channel);
/* 清空整个通知队列；需用户 y 确认 */
bool   notice_clear(NoticeService* s);

/* 通知管理菜单循环；B 在 NoticeService.c 末尾实现 */
void   notice_service_menu(NoticeService* s);

#endif /* NOTICE_SERVICE_H */
