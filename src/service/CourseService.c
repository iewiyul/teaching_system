/*
负责人：陆奕炜
*/
#include "CourseService.h"

CourseList* courselist_create(void)               { /* TODO(组员): 实现 */ return NULL; }
void        courselist_destroy(CourseList* l)      { /* TODO(组员): 实现 */ (void)l; }
bool        courselist_insert(CourseList* l, Course c)               { /* TODO(组员): 实现 */ (void)l; (void)c; return false; }
bool        courselist_remove(CourseList* l, const char* id)        { /* TODO(组员): 实现 */ (void)l; (void)id; return false; }
bool        courselist_update(CourseList* l, const char* id, Course c) { /* TODO(组员): 实现 */ (void)l; (void)id; (void)c; return false; }
Course*     courselist_find(CourseList* l, const char* id)          { /* TODO(组员): 实现 */ (void)l; (void)id; return NULL; }
Course*     courselist_at(CourseList* l, int index)                 { /* TODO(组员): 实现 */ (void)l; (void)index; return NULL; }
int         courselist_size(CourseList* l)                          { /* TODO(组员): 实现 */ (void)l; return 0; }

CourseService* course_service_create(SeqList* teachers, LinkedList* students, ScoreService* scores) {
    /* TODO(组员): 实现 */
    (void)teachers; (void)students; (void)scores; return NULL;
}
void           course_service_destroy(CourseService* s) { /* TODO(组员): 实现 */ (void)s; }

bool     course_add(CourseService* s, Course c)                                  { /* TODO(组员): 实现 */ (void)s; (void)c; return false; }
bool     course_remove(CourseService* s, const char* id)                         { /* TODO(组员): 实现 */ (void)s; (void)id; return false; }
bool     course_update(CourseService* s, const char* id, Course c)                { /* TODO(组员): 实现 */ (void)s; (void)id; (void)c; return false; }
Course*  course_queryById(CourseService* s, const char* id)                      { /* TODO(组员): 实现 */ (void)s; (void)id; return NULL; }
void     course_listAll(CourseService* s)                                        { /* TODO(组员): 实现 */ (void)s; }

bool     course_assignTeacher(CourseService* s, const char* courseId, const char* teacherId, double rating)   { /* TODO(组员): 实现 */ (void)s; (void)courseId; (void)teacherId; (void)rating; return false; }
bool     course_unassignTeacher(CourseService* s, const char* courseId, const char* teacherId)               { /* TODO(组员): 实现 */ (void)s; (void)courseId; (void)teacherId; return false; }
void     course_queryTeacherCourses(CourseService* s, const char* teacherId)                                  { /* TODO(组员): 实现 */ (void)s; (void)teacherId; }
void     course_queryCourseTeachers(CourseService* s, const char* courseId)                                   { /* TODO(组员): 实现 */ (void)s; (void)courseId; }

bool     course_addStudent(CourseService* s, const char* courseId, const char* studentId)                     { /* TODO(组员): 实现 */ (void)s; (void)courseId; (void)studentId; return false; }
bool     course_removeStudent(CourseService* s, const char* courseId, const char* studentId)                  { /* TODO(组员): 实现 */ (void)s; (void)courseId; (void)studentId; return false; }
void     course_queryStudentCourses(CourseService* s, const char* studentId)                                  { /* TODO(组员): 实现 */ (void)s; (void)studentId; }
void     course_queryCourseStudents(CourseService* s, const char* courseId)                                   { /* TODO(组员): 实现 */ (void)s; (void)courseId; }

Graph*       course_service_getGraph(CourseService* s)  { /* TODO(组员): 实现 */ (void)s; return NULL; }
CourseList*  course_service_getList(CourseService* s)   { /* TODO(组员): 实现 */ (void)s; return NULL; }

void        course_service_menu(CourseService* s) {
    /* TODO(组员): 实现菜单循环（README §6.3 菜单 13 项） */
    (void)s;
}
