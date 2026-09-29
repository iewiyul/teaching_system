/*
查找服务：组合 HashTable / TeacherHashTable / BST 三种结构
负责人：成员 D
*/
#ifndef SEARCH_SERVICE_H
#define SEARCH_SERVICE_H

#include "../common.h"
#include "../entity/Student.h"
#include "../entity/Teacher.h"
#include "../ds/HashTable.h"
#include "../ds/TeacherHashTable.h"
#include "../ds/BST.h"

/* 前向声明 */
struct StudentService;
struct TeacherService;
typedef struct StudentService StudentService;
typedef struct TeacherService TeacherService;

typedef struct SearchService {
    HashTable*        studentsById;
    TeacherHashTable* teachersById;
    BST*              studentsByGpa;
    BST*              studentsByBadLevel;

    StudentService* studentSvc;
    TeacherService* teacherSvc;
} SearchService;

/* 创建 SearchService；内部 new 学生哈希表 / 教师哈希表 / 绩点 BST / 不良级别 BST
   运行时依赖：ss（StudentService）、ts（TeacherService）——rebuildIndexes 时遍历源数据建索引 */
SearchService* search_service_create(StudentService* ss, TeacherService* ts);
/* 销毁并释放内部 4 个数据结构 */
void           search_service_destroy(SearchService* s);

/* 从 StudentService / TeacherService 重建全部查找索引
   （init_seed_data 末尾必须调用一次，否则哈希/BST 是空的） */
void   search_service_rebuildIndexes(SearchService* s);

/* 按学号在学生哈希表中精确查找（打印桶号 + ⏱️ 耗时） */
Student*  search_findStudentById(SearchService* s, const char* id);
/* 按工号在教师哈希表中精确查找（打印桶号 + ⏱️ 耗时） */
Teacher*  search_findTeacherById(SearchService* s, const char* id);
/* 在绩点 BST 上做范围 [low, high] 查询（⏱️ 耗时） */
void      search_findStudentsByGPA(SearchService* s, double low, double high);
/* 在不良级别 BST 上做范围 [low, high] 查询（⏱️ 耗时） */
void      search_findStudentsByBadLevel(SearchService* s, int low, int high);
/* 性能对比演示：哈希 O(1) vs BST 中序遍历（验收亮点） */
void      search_benchmark(SearchService* s, int testCount);
/* 打印学生哈希表状态（容量、装填因子、最长链、桶分布图） */
void      search_printHashStats(SearchService* s);

/* 查找服务菜单循环；D 在 SearchService.c 末尾实现 */
void      search_service_menu(SearchService* s);

#endif /* SEARCH_SERVICE_H */
