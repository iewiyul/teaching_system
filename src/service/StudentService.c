/*
负责人：成员 C
*/
#include "StudentService.h"

StudentService* student_service_create(void)             { /* TODO(组员): 实现 */ return NULL; }
void            student_service_destroy(StudentService* s) { /* TODO(组员): 实现 */ (void)s; }

bool     student_add(StudentService* s, Student stu)                       { /* TODO(组员): 实现 */ (void)s; (void)stu; return false; }
bool     student_remove(StudentService* s, const char* id)                { /* TODO(组员): 实现 */ (void)s; (void)id; return false; }
bool     student_update(StudentService* s, const char* id, Student stu)   { /* TODO(组员): 实现 */ (void)s; (void)id; (void)stu; return false; }
Student* student_queryById(StudentService* s, const char* id)             { /* TODO(组员): 实现 */ (void)s; (void)id; return NULL; }
void     student_listAll(StudentService* s)                               { /* TODO(组员): 实现 */ (void)s; }
void     student_sortByGpaDesc(StudentService* s, int k)                  { /* TODO(组员): 实现 */ (void)s; (void)k; }
void     student_listBadRecords(StudentService* s)                        { /* TODO(组员): 实现 */ (void)s; }

void     student_service_menu(StudentService* s) {
    /* TODO(组员): 实现菜单循环（README §8.5.2） */
    (void)s;
}
