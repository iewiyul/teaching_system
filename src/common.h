/*
公共头文件————涵盖宏定义和通用的函数
负责人：陆奕炜
*/
#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

/* ---------- 字符串与容量常量 ---------- */
#define MAX_ID_LEN     20    /* 工号/学号/课程号等 ID 字段长度 */
#define MAX_NAME_LEN   50    /* 姓名/名称字段长度 */
#define MAX_TEXT_LEN   256   /* 通知/描述等长文本字段长度 */

/* ---------- 顶点容量上限（图用） ---------- */
#define MAX_VERTEX     100   /* Graph 中允许的最大顶点数（参考 README §6.2） */

/* ---------- 时间格式化 ---------- */
/*
 * 把 time_t 格式化成 "YYYY-MM-DD HH:MM:SS"，写入 size>=32 的 buf。
 * 声明在 common.h，实现在 src/common.c（陆奕炜 维护的跨层工具函数）。
 */
void format_time(long ts, char* buf);

/* ---------- 字符串哈希（散列函数） ---------- */
/*
 * 对 char* 字符串做多项式哈希，返回非负整数。
 * 声明在 common.h，实现在 src/common.c；任何 .c 通过 #include "common.h" 调签名。
 */
unsigned int hash_str(const char* s);

/* ---------- 跨服务回调：操作日志写入 ---------- */
/*
 * 把一条操作写入全局 OperationHistory 栈。
 * 声明在 common.h，实现在 src/common.c；A/C/D/E 在各自 add/remove/update 中调用一次。
 * 签名固定，不要随意修改；新增回调统一在 README §6.4 追加。
 */
void history_record(const char* opType, const char* entityType,
                    const char* targetId, const char* desc);

#endif /* COMMON_H */
