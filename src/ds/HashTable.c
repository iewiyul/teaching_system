/*
负责人：成员 D
*/
#include "HashTable.h"

HashTable* hashtable_create(int capacity)               { /* TODO(组员): 实现 */ (void)capacity; return NULL; }
void       hashtable_destroy(HashTable* ht)             { /* TODO(组员): 实现 */ (void)ht; }

bool       hashtable_insert(HashTable* ht, const char* key, Student* s)  { /* TODO(组员): 实现 */ (void)ht; (void)key; (void)s; return false; }
bool       hashtable_remove(HashTable* ht, const char* key)              { /* TODO(组员): 实现 */ (void)ht; (void)key; return false; }
Student*   hashtable_find(HashTable* ht, const char* key)                { /* TODO(组员): 实现 */ (void)ht; (void)key; return NULL; }
bool       hashtable_contains(HashTable* ht, const char* key)            { /* TODO(组员): 实现 */ (void)ht; (void)key; return false; }
int        hashtable_size(HashTable* ht)                                 { /* TODO(组员): 实现 */ (void)ht; return 0; }

int        hashtable_getLongestChain(HashTable* ht) { /* TODO(组员): 实现 */ (void)ht; return 0; }
double     hashtable_getLoadFactor(HashTable* ht)   { /* TODO(组员): 实现 */ (void)ht; return 0.0; }
void       hashtable_printStats(HashTable* ht)      { /* TODO(组员): 实现 */ (void)ht; }
