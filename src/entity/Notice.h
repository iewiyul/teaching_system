/*
实体定义：通知
负责人：陆奕炜
*/
#ifndef NOTICE_H
#define NOTICE_H

#include "../common.h"

typedef struct {
    char   id[MAX_ID_LEN];       /* 通知 ID（可由系统生成或用户输入） */
    char   title[100];           /* 标题 */
    char   content[MAX_TEXT_LEN];/* 内容 */
    char   channel[10];          /* 通道：SMS / EMAIL / APP */
    char   target[MAX_ID_LEN];   /* 接收者 ID（教师/学生/ALL） */
    bool   sent;                 /* 是否已发送 */
} Notice;

#endif /* NOTICE_H */
