/*
排行榜：底层用 Heap 做 TopK
负责人：成员 E
*/
#ifndef RANKING_SERVICE_H
#define RANKING_SERVICE_H

#include "../common.h"
#include "../entity/Student.h"
#include "../entity/Teacher.h"
#include "../ds/Heap.h"

/* 前向声明 */
struct StudentService;
struct TeacherService;
typedef struct StudentService StudentService;
typedef struct TeacherService TeacherService;

typedef struct RankingService {
    StudentService* studentSvc;   /* 引用（不强拥有） */
    TeacherService* teacherSvc;
} RankingService;

/* 创建 RankingService（无内部数据结构，仅持有外部 Service 引用）
   运行时依赖：ss（StudentService，取学生绩点做 TopK）、
                ts（TeacherService，取教师评分做 TopK） */
RankingService* ranking_service_create(StudentService* ss, TeacherService* ts);
/* 销毁 RankingService（仅 free 自身，不释放两个外部引用） */
void            ranking_service_destroy(RankingService* r);

/* 学生绩点 TopK（堆 TopK 算法，O(n log k)）；运行时从 ss->students 取全部数据 */
void   ranking_topKStudents(StudentService* ss, int k);
/* 教师授课评分 TopK（堆 TopK 算法）；运行时从 ts->teachers 取全部数据 */
void   ranking_topKTeachers(TeacherService* ts, int k);

/* 排行榜菜单循环；E 在 RankingService.c 末尾实现 */
void   ranking_service_menu(RankingService* r);

#endif /* RANKING_SERVICE_H */
