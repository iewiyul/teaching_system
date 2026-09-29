/*
负责人：成员 B
*/
#include "Stack.h"

Stack*  stack_create(void)                { /* TODO(组员): 实现 */ return NULL; }
void   stack_destroy(Stack* s)            { /* TODO(组员): 实现 */ (void)s; }

bool   stack_push(Stack* s, Record r)     { /* TODO(组员): 实现 */ (void)s; (void)r; return false; }
Record stack_pop(Stack* s)                { /* TODO(组员): 实现 */ (void)s; Record z = {0}; return z; }
Record stack_peek(Stack* s)               { /* TODO(组员): 实现 */ (void)s; Record z = {0}; return z; }

bool   stack_isEmpty(Stack* s)            { /* TODO(组员): 实现 */ (void)s; return true; }
int    stack_size(Stack* s)               { /* TODO(组员): 实现 */ (void)s; return 0; }
void   stack_clear(Stack* s)              { /* TODO(组员): 实现 */ (void)s; }
