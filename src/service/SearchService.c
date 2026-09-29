/*
负责人：成员 D
*/
#include "SearchService.h"

SearchService* search_service_create(StudentService* ss, TeacherService* ts) {
    /* TODO(组员): 实现 */
    (void)ss; (void)ts; return NULL;
}
void           search_service_destroy(SearchService* s) { /* TODO(组员): 实现 */ (void)s; }

void   search_service_rebuildIndexes(SearchService* s) { /* TODO(组员): 实现 */ (void)s; }

Student*  search_findStudentById(SearchService* s, const char* id)        { /* TODO(组员): 实现 */ (void)s; (void)id; return NULL; }
Teacher*  search_findTeacherById(SearchService* s, const char* id)        { /* TODO(组员): 实现 */ (void)s; (void)id; return NULL; }
void      search_findStudentsByGPA(SearchService* s, double low, double high)        { /* TODO(组员): 实现 */ (void)s; (void)low; (void)high; }
void      search_findStudentsByBadLevel(SearchService* s, int low, int high)          { /* TODO(组员): 实现 */ (void)s; (void)low; (void)high; }
void      search_benchmark(SearchService* s, int testCount)              { /* TODO(组员): 实现 */ (void)s; (void)testCount; }
void      search_printHashStats(SearchService* s)                        { /* TODO(组员): 实现 */ (void)s; }

void      search_service_menu(SearchService* s) {
    /* TODO(组员): 实现菜单循环（README §9.4） */
    (void)s;
}
