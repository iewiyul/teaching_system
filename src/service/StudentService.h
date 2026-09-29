/*
学生业务：底层用 LinkedList 存 Student
负责人：成员 C
*/
#ifndef STUDENT_SERVICE_H
#define STUDENT_SERVICE_H

#include "../common.h"
#include "../entity/Student.h"
#include "../ds/LinkedList.h"

typedef struct StudentService {
    LinkedList* students;
} StudentService;

/* 创建 StudentService；内部 new 一个 LinkedList 存 Student */
StudentService* student_service_create(void);
/* 销毁并释放内部 LinkedList（含所有节点） */
void            student_service_destroy(StudentService* s);

/* 添加学生；成功后 history_record("add", "Student", stu.id) */
bool      student_add(StudentService* s, Student stu);
/* 按学号删除学生；成功后 history_record("remove", "Student", id) */
bool      student_remove(StudentService* s, const char* id);
/* 按学号替换为 stu；成功后 history_record("update", "Student", id) */
bool      student_update(StudentService* s, const char* id, Student stu);
/* 按学号查找学生；返回链表内节点指针（生命周期同 Service） */
Student*  student_queryById(StudentService* s, const char* id);
/* 分页遍历打印全部学生（含人数统计） */
void      student_listAll(StudentService* s);
/* 按绩点降序排序后打印前 k 名（复制链表再排，耗时统计） */
void      student_sortByGpaDesc(StudentService* s, int k);
/* 列出 hasBadRecord=true 的学生（学号、姓名、不良级别、绩点） */
void      student_listBadRecords(StudentService* s);

/* 学生管理菜单循环；C 在 StudentService.c 末尾实现 */
void      student_service_menu(StudentService* s);

#endif /* STUDENT_SERVICE_H */
