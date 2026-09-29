/*
课程业务：底层用 CourseList 存 Course；任课/选课关系用 Graph
负责人：陆奕炜
*/
#ifndef COURSE_SERVICE_H
#define COURSE_SERVICE_H

#include "../common.h"
#include "../entity/Course.h"
#include "../ds/SeqList.h"
#include "../ds/LinkedList.h"
#include "../ds/Graph.h"

/* ----- CourseList：与 SeqList 平行的容器（存 Course 而非泛型） ----- */
typedef struct CourseList {
    Course* data;
    int     size;
    int     capacity;
} CourseList;

/* 创建空 CourseList */
CourseList* courselist_create(void);
/* 销毁并释放内部 Course 数组 */
void        courselist_destroy(CourseList* l);
/* 末尾追加 Course；容量不足自动扩容 */
bool        courselist_insert(CourseList* l, Course c);
/* 按课程号删除首个匹配项 */
bool        courselist_remove(CourseList* l, const char* id);
/* 按课程号替换为新 Course */
bool        courselist_update(CourseList* l, const char* id, Course c);
/* 按课程号查找；返回内部指针（不要 free） */
Course*     courselist_find(CourseList* l, const char* id);
/* 按下标取 Course 指针；越界返回 NULL */
Course*     courselist_at(CourseList* l, int index);
/* 当前 CourseList 元素个数 */
int         courselist_size(CourseList* l);

/* 前向声明，避免循环依赖 */
struct ScoreService;
typedef struct ScoreService ScoreService;

/* ----- CourseService 主结构 ----- */
typedef struct CourseService {
    CourseList*   courses;
    Graph*        courseGraph;
    SeqList*      teachersRef;
    LinkedList*   studentsRef;
    ScoreService* scoresRef;
} CourseService;

/* 创建 CourseService；内部 new CourseList 与 Graph
   运行时依赖：teachers（Teacher SeqList，按工号显示教师姓名）、
                students（Student LinkedList，按学号显示学生姓名）、
                scores（ScoreService，通过 scorelist_find 取单科成绩） */
CourseService* course_service_create(SeqList* teachers,
                                     LinkedList* students,
                                     ScoreService* scores);
/* 销毁并释放内部 CourseList 与 Graph（不释放三个外部引用） */
void           course_service_destroy(CourseService* s);

/* 新增课程；同时把课程 id 加入 Graph 顶点；成功后 history_record("add", "Course", c.id) */
bool     course_add(CourseService* s, Course c);
/* 按课程号删除课程；清理图中所有相关边；需用户 y 确认；
   成功后 history_record("remove", "Course", id) */
bool     course_remove(CourseService* s, const char* id);
/* 按课程号替换课程信息；成功后 history_record("update", "Course", id) */
bool     course_update(CourseService* s, const char* id, Course c);
/* 按课程号查询；同时从 Graph 列出该课程的任课教师（前 10 名选课学生）；
   用到 teachersRef / studentsRef / scoresRef */
Course*  course_queryById(CourseService* s, const char* id);
/* 表格格式遍历打印全部课程 */
void     course_listAll(CourseService* s);

/* 建立课程←教师任课边（EDGE_TEACH，权重=rating）；
   校验课程/教师存在；成功后 history_record("add", "Edge", courseId) */
bool     course_assignTeacher(CourseService* s, const char* courseId,
                              const char* teacherId, double rating);
/* 解除课程←教师任课边；需用户 y 确认；成功后 history_record("remove", "Edge", courseId) */
bool     course_unassignTeacher(CourseService* s, const char* courseId,
                                const char* teacherId);
/* 查询某教师的所有任课课程（按图遍历 EDGE_TEACH 边）；
   用到 teachersRef 取教师姓名 */
void     course_queryTeacherCourses(CourseService* s, const char* teacherId);
/* 查询某课程的所有任课教师（按图遍历 EDGE_TEACH 边）；
   用到 teachersRef 取教师姓名 */
void     course_queryCourseTeachers(CourseService* s, const char* courseId);

/* 建立课程←学生选课边（EDGE_TAKE）；校验课程/学生存在；
   成功后 history_record("add", "Edge", courseId) */
bool     course_addStudent(CourseService* s, const char* courseId,
                           const char* studentId);
/* 解除课程←学生选课边；需用户 y 确认；成功后 history_record("remove", "Edge", courseId) */
bool     course_removeStudent(CourseService* s, const char* courseId,
                              const char* studentId);
/* 查询某学生的所有选课（按图遍历 EDGE_TAKE 边）；
   同时通过 scoresRef（scorelist_find）取每门课成绩 */
void     course_queryStudentCourses(CourseService* s, const char* studentId);
/* 查询某课程的所有选课学生（前 10 条 + 全部提示）；
   用到 studentsRef 取学生姓名与绩点 */
void     course_queryCourseStudents(CourseService* s, const char* courseId);

/* 暴露 Graph 句柄，供外部（seed.c 等）建边/查询 */
Graph*       course_service_getGraph(CourseService* s);
/* 暴露 CourseList 句柄，供外部（seed.c 等）读取/遍历课程 */
CourseList*  course_service_getList(CourseService* s);

/* 课程管理菜单循环；陆奕炜（A）在 CourseService.c 末尾实现 */
void        course_service_menu(CourseService* s);

#endif /* COURSE_SERVICE_H */
