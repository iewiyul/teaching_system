/*
负责人：成员 E
*/
#include "ScoreService.h"

ScoreList* scorelist_create(void)                                  { /* TODO(组员): 实现 */ return NULL; }
void       scorelist_destroy(ScoreList* l)                         { /* TODO(组员): 实现 */ (void)l; }
bool       scorelist_insert(ScoreList* l, Score sc)                { /* TODO(组员): 实现 */ (void)l; (void)sc; return false; }
bool       scorelist_remove(ScoreList* l, const char* studentId, const char* courseId) { /* TODO(组员): 实现 */ (void)l; (void)studentId; (void)courseId; return false; }
Score*     scorelist_find(ScoreList* l, const char* studentId, const char* courseId)   { /* TODO(组员): 实现 */ (void)l; (void)studentId; (void)courseId; return NULL; }
Score*     scorelist_at(ScoreList* l, int index)                   { /* TODO(组员): 实现 */ (void)l; (void)index; return NULL; }
int        scorelist_size(ScoreList* l)                            { /* TODO(组员): 实现 */ (void)l; return 0; }

ScoreService* score_service_create(StudentService* ss, CourseService* cs) {
    /* TODO(组员): 实现 */
    (void)ss; (void)cs; return NULL;
}
void          score_service_destroy(ScoreService* s) { /* TODO(组员): 实现 */ (void)s; }

bool   score_add(ScoreService* s, Score sc)                                              { /* TODO(组员): 实现 */ (void)s; (void)sc; return false; }
bool   score_remove(ScoreService* s, const char* studentId, const char* courseId)        { /* TODO(组员): 实现 */ (void)s; (void)studentId; (void)courseId; return false; }
void   score_findByStudent(ScoreService* s, const char* studentId)                       { /* TODO(组员): 实现 */ (void)s; (void)studentId; }
void   score_findByCourse(ScoreService* s, const char* courseId)                         { /* TODO(组员): 实现 */ (void)s; (void)courseId; }
void   score_sortByScoreDesc(ScoreService* s, int k)                                     { /* TODO(组员): 实现 */ (void)s; (void)k; }
void   score_avgByStudent(ScoreService* s, const char* studentId)                        { /* TODO(组员): 实现 */ (void)s; (void)studentId; }
void   score_avgByCourse(ScoreService* s, const char* courseId)                          { /* TODO(组员): 实现 */ (void)s; (void)courseId; }

void   score_service_menu(ScoreService* s) {
    /* TODO(组员): 实现菜单循环（README §10.5.1） */
    (void)s;
}
