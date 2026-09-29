/*
实体定义：操作历史记录（Stack 元素）
负责人：陆奕炜
*/
#ifndef RECORD_H
#define RECORD_H

#include "../common.h"

typedef struct {
    char   opType[10];           /* 操作类型：add / remove / update / send */
    char   entityType[20];       /* 实体类型：Teacher / Student / Course / ... */
    char   targetId[MAX_ID_LEN]; /* 操作目标的 ID */
    char   description[100];     /* 操作描述 */
    long   timestamp;            /* 时间戳（time_t） */
} Record;

#endif /* RECORD_H */
