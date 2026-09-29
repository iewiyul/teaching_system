/*
实体定义：课程
负责人：陆奕炜
*/
#ifndef COURSE_H
#define COURSE_H

#include "../common.h"

typedef struct {
    char   id[MAX_ID_LEN];       /* 课程号，如 "C001" */
    char   name[MAX_NAME_LEN];   /* 课程名 */
    double credit;               /* 学分 */
    int    hours;                /* 学时 */
    char   semester[20];         /* 学期，如 "2024秋" */
} Course;

#endif /* COURSE_H */
