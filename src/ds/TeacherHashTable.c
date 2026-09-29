/*
负责人：成员 D
*/
#include "TeacherHashTable.h"

TeacherHashTable* teacher_hashtable_create(int capacity)              { /* TODO(组员): 实现 */ (void)capacity; return NULL; }
void              teacher_hashtable_destroy(TeacherHashTable* ht)      { /* TODO(组员): 实现 */ (void)ht; }

bool              teacher_hashtable_insert(TeacherHashTable* ht, const char* key, Teacher* t) { /* TODO(组员): 实现 */ (void)ht; (void)key; (void)t; return false; }
bool              teacher_hashtable_remove(TeacherHashTable* ht, const char* key)             { /* TODO(组员): 实现 */ (void)ht; (void)key; return false; }
Teacher*          teacher_hashtable_find(TeacherHashTable* ht, const char* key)               { /* TODO(组员): 实现 */ (void)ht; (void)key; return NULL; }
bool              teacher_hashtable_contains(TeacherHashTable* ht, const char* key)           { /* TODO(组员): 实现 */ (void)ht; (void)key; return false; }
int               teacher_hashtable_size(TeacherHashTable* ht)                                { /* TODO(组员): 实现 */ (void)ht; return 0; }
