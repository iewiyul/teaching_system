/*
负责人：成员 C
*/
#include "LinkedList.h"

LinkedList* linkedlist_create(void)              { /* TODO(组员): 实现 */ return NULL; }
void        linkedlist_destroy(LinkedList* l)     { /* TODO(组员): 实现 */ (void)l; }

bool     linkedlist_insert(LinkedList* l, Student s)      { /* TODO(组员): 实现 */ (void)l; (void)s; return false; }
bool     linkedlist_insertTail(LinkedList* l, Student s)  { /* TODO(组员): 实现 */ (void)l; (void)s; return false; }
bool     linkedlist_remove(LinkedList* l, const char* id) { /* TODO(组员): 实现 */ (void)l; (void)id; return false; }
Student* linkedlist_find(LinkedList* l, const char* id)   { /* TODO(组员): 实现 */ (void)l; (void)id; return NULL; }

void     linkedlist_sort_by_gpa_desc(LinkedList* l) { /* TODO(组员): 实现 */ (void)l; }

void     linkedlist_traverse(LinkedList* l)   { /* TODO(组员): 实现 */ (void)l; }
int      linkedlist_size(LinkedList* l)       { /* TODO(组员): 实现 */ (void)l; return 0; }
bool     linkedlist_isEmpty(LinkedList* l)    { /* TODO(组员): 实现 */ (void)l; return true; }
