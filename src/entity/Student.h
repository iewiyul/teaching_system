/*
实体定义：学生
负责人：陆奕炜
*/
#ifndef STUDENT_H
#define STUDENT_H

#include "../common.h"

typedef struct {
    char   id[MAX_ID_LEN];       /* 学号，如 "S2023001" */
    char   name[MAX_NAME_LEN];   /* 姓名 */
    char   gender[4];            /* 性别：男 / 女 */
    char   major[50];            /* 专业 */
    int    grade;                /* 年级 */
    double gpa;                  /* 绩点 0~5 */
    bool   hasBadRecord;         /* 是否有不良记录 */
    int    badLevel;             /* 不良记录严重程度 0~3 */
} Student;

#endif /* STUDENT_H */
