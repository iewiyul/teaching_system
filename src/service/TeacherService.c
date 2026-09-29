/*
负责人：成员 C
*/
#include "TeacherService.h"

TeacherService* teacher_service_create(void)             { /* TODO(组员): 实现 */ return NULL; }
void            teacher_service_destroy(TeacherService* s) { /* TODO(组员): 实现 */ (void)s; }

bool     teacher_add(TeacherService* s, Teacher t)                       { /* TODO(组员): 实现 */ (void)s; (void)t; return false; }
bool     teacher_remove(TeacherService* s, const char* id)              { /* TODO(组员): 实现 */ (void)s; (void)id; return false; }
bool     teacher_update(TeacherService* s, const char* id, Teacher t)   { /* TODO(组员): 实现 */ (void)s; (void)id; (void)t; return false; }
Teacher* teacher_queryById(TeacherService* s, const char* id)            { /* TODO(组员): 实现 */ (void)s; (void)id; return NULL; }
void     teacher_listAll(TeacherService* s)                             { /* TODO(组员): 实现 */ (void)s; }
void     teacher_sortByRatingDesc(TeacherService* s)                    { /* TODO(组员): 实现 */ (void)s; }

void     teacher_service_menu(TeacherService* s) {
    /* TODO(组员): 实现菜单循环（README §8.5.1） */
    (void)s;
}
