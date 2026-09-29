/*
实体定义：教师
负责人：陆奕炜
*/
#ifndef TEACHER_H
#define TEACHER_H

#include "../common.h"

typedef struct {
    char   id[MAX_ID_LEN];       /* 工号，如 "T001" */
    char   name[MAX_NAME_LEN];   /* 姓名 */
    char   title[20];            /* 职称：讲师 / 副教授 / 教授 */
    char   department[50];       /* 院系 */
    double rating;               /* 授课评分 0~5 */
} Teacher;

#endif /* TEACHER_H */
