/*
实体定义：成绩记录
负责人：陆奕炜
*/
#ifndef SCORE_H
#define SCORE_H

#include "../common.h"

typedef struct {
    char   studentId[MAX_ID_LEN]; /* 学号 */
    char   courseId[MAX_ID_LEN];  /* 课程号 */
    double value;                 /* 分数 0~100 */
    char   examType[20];          /* 期中 / 期末 / 平时 */
} Score;

#endif /* SCORE_H */
