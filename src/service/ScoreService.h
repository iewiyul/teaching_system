/*
成绩业务：底层用 ScoreList 存 Score；排序用 Heap（堆排序）
负责人：成员 E
*/
#ifndef SCORE_SERVICE_H
#define SCORE_SERVICE_H

#include "../common.h"
#include "../entity/Score.h"
#include "../entity/Student.h"
#include "../entity/Course.h"
#include "../ds/Heap.h"

/* ----- ScoreList：与 CourseList 平行的容器（存 Score 而非泛型） ----- */
typedef struct ScoreList {
    Score* data;
    int    size;
    int    capacity;
} ScoreList;

/* 创建空 ScoreList */
ScoreList* scorelist_create(void);
/* 销毁并释放内部 Score 数组 */
void       scorelist_destroy(ScoreList* l);
/* 末尾追加 Score；容量不足自动扩容 */
bool       scorelist_insert(ScoreList* l, Score sc);
/* 按 (studentId, courseId) 复合键删除首个匹配项 */
bool       scorelist_remove(ScoreList* l, const char* studentId, const char* courseId);
/* 按 (studentId, courseId) 复合键查找；返回内部指针（不要 free） */
Score*     scorelist_find(ScoreList* l, const char* studentId, const char* courseId);
/* 按下标取 Score 指针；越界返回 NULL */
Score*     scorelist_at(ScoreList* l, int index);
/* 当前 ScoreList 元素个数 */
int        scorelist_size(ScoreList* l);

/* 前向声明 */
struct StudentService;
struct CourseService;
typedef struct StudentService StudentService;
typedef struct CourseService  CourseService;

/* ----- ScoreService 主结构 ----- */
typedef struct ScoreService {
    ScoreList*      scores;
    StudentService* studentSvc;   /* 引用，按学号查姓名 */
    CourseService*  courseSvc;    /* 引用，按课程号查课程名 */
} ScoreService;

/* 创建 ScoreService；内部 new ScoreList
   运行时依赖：ss（StudentService，按学号查姓名）、cs（CourseService，按课程号查课程名） */
ScoreService* score_service_create(StudentService* ss, CourseService* cs);
/* 销毁并释放内部 ScoreList（不释放两个外部引用） */
void          score_service_destroy(ScoreService* s);

/* 新增成绩记录；成功后 history_record("add", "Score", sc.studentId) */
bool   score_add(ScoreService* s, Score sc);
/* 按 (studentId, courseId) 删除成绩；成功后 history_record("remove", "Score", studentId) */
bool   score_remove(ScoreService* s, const char* studentId, const char* courseId);
/* 按学号查询某学生全部成绩（含课程名、平均分）；
   用到 studentSvc（查姓名）和 courseSvc（查课程名） */
void   score_findByStudent(ScoreService* s, const char* studentId);
/* 按课程号查询某课程全部成绩（含学号姓名、平均分）；
   用到 studentSvc（查姓名）和 courseSvc（查课程名） */
void   score_findByCourse(ScoreService* s, const char* courseId);
/* 把全部 Score 拷贝进数组做堆排序（降序），打印前 k 条（耗时统计） */
void   score_sortByScoreDesc(ScoreService* s, int k);
/* 计算某学生平均分并按内部 score→gpa 表换算绩点；
   用到 studentSvc（查姓名） */
void   score_avgByStudent(ScoreService* s, const char* studentId);
/* 计算某课程平均分；用到 courseSvc（查课程名） */
void   score_avgByCourse(ScoreService* s, const char* courseId);

/* 成绩管理菜单循环；E 在 ScoreService.c 末尾实现 */
void   score_service_menu(ScoreService* s);

#endif /* SCORE_SERVICE_H */
