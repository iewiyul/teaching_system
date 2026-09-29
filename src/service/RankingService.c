/*
负责人：成员 E
*/
#include "RankingService.h"

RankingService* ranking_service_create(StudentService* ss, TeacherService* ts) {
    /* TODO(组员): 实现 */
    (void)ss; (void)ts; return NULL;
}
void            ranking_service_destroy(RankingService* r) { /* TODO(组员): 实现 */ (void)r; }

void   ranking_topKStudents(StudentService* ss, int k) { /* TODO(组员): 实现 */ (void)ss; (void)k; }
void   ranking_topKTeachers(TeacherService* ts, int k) { /* TODO(组员): 实现 */ (void)ts; (void)k; }

void   ranking_service_menu(RankingService* r) {
    /* TODO(组员): 实现菜单循环（README §10.5.2） */
    (void)r;
}
