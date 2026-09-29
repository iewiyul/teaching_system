/*
教师业务：底层用 SeqList 存 Teacher
负责人：成员 C
*/
#ifndef TEACHER_SERVICE_H
#define TEACHER_SERVICE_H

#include "../common.h"
#include "../ds/SeqList.h"
#include "../entity/Teacher.h"

typedef struct TeacherService {
    SeqList *teachers;
} TeacherService;

/* 创建 TeacherService；内部 new 一个 SeqList 存 Teacher */
TeacherService *teacher_service_create(void);
/* 销毁并释放内部 SeqList（含 Teacher 数组） */
void teacher_service_destroy(TeacherService *s);

/* 添加教师；成功后 history_record("add", "Teacher", t.id) */
bool teacher_add(TeacherService *s, Teacher t);
/* 按工号删除教师；成功后 history_record("remove", "Teacher", id) */
bool teacher_remove(TeacherService *s, const char *id);
/* 按工号替换为 t；成功后 history_record("update", "Teacher", id) */
bool teacher_update(TeacherService *s, const char *id, Teacher t);
/* 按工号查找教师；返回 SeqList 内指针（生命周期同 Service） */
Teacher *teacher_queryById(TeacherService *s, const char *id);
/* 表格格式遍历打印全部教师（含人数统计） */
void teacher_listAll(TeacherService *s);
/* 按授课评分降序排序后打印（耗时统计） */
void teacher_sortByRatingDesc(TeacherService *s);

/* 教师管理菜单循环；C 在 TeacherService.c 末尾实现 */
void teacher_service_menu(TeacherService *s);

#endif /* TEACHER_SERVICE_H */
