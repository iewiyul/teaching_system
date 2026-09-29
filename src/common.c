/*
 * 负责人：陆奕炜
 */
#include "common.h"

#include "entity/Record.h"
#include "ds/Stack.h"
#include "service/OperationHistory.h"

/* history_record 通过全局 OperationHistory 写入历史栈
 * 全局在 main.c 定义，本文件 extern 引用 */
extern OperationHistory* g_operationHistory;

unsigned int hash_str(const char* s) {
    unsigned int h = 0;
    if (!s) return 0;
    while (*s) {
        h = h * 131u + (unsigned char)(*s);
        s++;
    }
    return h;
}

void format_time(long ts, char* buf) {
    if (!buf) return;
    time_t t = (time_t)ts;
    struct tm tm_buf;
    localtime_s(&tm_buf, &t);
    strftime(buf, 32, "%Y-%m-%d %H:%M:%S", &tm_buf);
}

void history_record(const char* opType, const char* entityType,
                    const char* targetId, const char* desc) {
    /* 防御：全局未初始化时不崩（main.c 启动早期误调等情况） */
    if (!g_operationHistory || !g_operationHistory->stack) return;

    Record r;
    memset(&r, 0, sizeof(r));
    strncpy(r.opType,      opType,                   sizeof(r.opType)      - 1);
    strncpy(r.entityType,  entityType,               sizeof(r.entityType)  - 1);
    strncpy(r.targetId,    targetId ? targetId : "", sizeof(r.targetId)    - 1);
    strncpy(r.description, desc,                     sizeof(r.description) - 1);
    r.timestamp = time(NULL);
    stack_push(g_operationHistory->stack, r);
}