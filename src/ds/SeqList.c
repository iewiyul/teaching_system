/*
负责人：成员 C
*/
#include "SeqList.h"

SeqList* seqlist_create(void)              { /* TODO(组员): 实现 */ return NULL; }
void     seqlist_destroy(SeqList* s)       { /* TODO(组员): 实现 */ (void)s; }

bool     seqlist_insert(SeqList* s, Teacher t)                 { /* TODO(组员): 实现 */ (void)s; (void)t; return false; }
bool     seqlist_remove(SeqList* s, const char* id)            { /* TODO(组员): 实现 */ (void)s; (void)id; return false; }
bool     seqlist_update(SeqList* s, const char* id, Teacher t) { /* TODO(组员): 实现 */ (void)s; (void)id; (void)t; return false; }
Teacher* seqlist_find(SeqList* s, const char* id)              { /* TODO(组员): 实现 */ (void)s; (void)id; return NULL; }
Teacher* seqlist_at(SeqList* s, int index)                     { /* TODO(组员): 实现 */ (void)s; (void)index; return NULL; }

void     seqlist_sort_by_id(SeqList* s)             { /* TODO(组员): 实现 */ (void)s; }
void     seqlist_sort_by_rating_desc(SeqList* s)    { /* TODO(组员): 实现 */ (void)s; }

void     seqlist_traverse(SeqList* s)   { /* TODO(组员): 实现 */ (void)s; }
int      seqlist_size(SeqList* s)       { /* TODO(组员): 实现 */ (void)s; return 0; }
bool     seqlist_isEmpty(SeqList* s)    { /* TODO(组员): 实现 */ (void)s; return true; }
