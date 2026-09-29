/*
负责人：成员 B
*/
#include "NoticeService.h"

NoticeService* notice_service_create(void)             { /* TODO(组员): 实现 */ return NULL; }
void           notice_service_destroy(NoticeService* s) { /* TODO(组员): 实现 */ (void)s; }

bool   notice_add(NoticeService* s, Notice n)                  { /* TODO(组员): 实现 */ (void)s; (void)n; return false; }
bool   notice_sendNext(NoticeService* s)                       { /* TODO(组员): 实现 */ (void)s; return false; }
void   notice_peek(NoticeService* s)                           { /* TODO(组员): 实现 */ (void)s; }
void   notice_listAll(NoticeService* s)                        { /* TODO(组员): 实现 */ (void)s; }
void   notice_filterByChannel(NoticeService* s, const char* ch){ /* TODO(组员): 实现 */ (void)s; (void)ch; }
bool   notice_clear(NoticeService* s)                          { /* TODO(组员): 实现 */ (void)s; return false; }

void   notice_service_menu(NoticeService* s) {
    /* TODO(组员): 实现菜单循环（README §7.6.1） */
    (void)s;
}
