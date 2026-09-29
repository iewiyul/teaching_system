/*
负责人：陆奕炜
*/
#include "Graph.h"

Graph*   graph_create(void)                                       { /* TODO(组员): 实现 */ return NULL; }
void     graph_destroy(Graph* g)                                  { /* TODO(组员): 实现 */ (void)g; }

bool     graph_addVertex(Graph* g, const char* id)                { /* TODO(组员): 实现 */ (void)g; (void)id; return false; }
bool     graph_addEdge(Graph* g, const char* fromId, const char* toId,
                       EdgeType type, double weight)              { /* TODO(组员): 实现 */ (void)g; (void)fromId; (void)toId; (void)type; (void)weight; return false; }
bool     graph_removeEdge(Graph* g, const char* fromId, const char* toId, EdgeType type) { /* TODO(组员): 实现 */ (void)g; (void)fromId; (void)toId; (void)type; return false; }
bool     graph_removeVertex(Graph* g, const char* id)             { /* TODO(组员): 实现 */ (void)g; (void)id; return false; }

void     graph_getNeighbors(Graph* g, const char* id,
                            char result[][MAX_ID_LEN], int* count)                       { /* TODO(组员): 实现 */ (void)g; (void)id; (void)result; (void)count; }
void     graph_getTeachersOfCourse(Graph* g, const char* courseId,
                                   char result[][MAX_ID_LEN], int* count)                { /* TODO(组员): 实现 */ (void)g; (void)courseId; (void)result; (void)count; }
void     graph_getStudentsOfCourse(Graph* g, const char* courseId,
                                   char result[][MAX_ID_LEN], int* count)                { /* TODO(组员): 实现 */ (void)g; (void)courseId; (void)result; (void)count; }
void     graph_getCoursesOfTeacher(Graph* g, const char* teacherId,
                                   char result[][MAX_ID_LEN], int* count)                { /* TODO(组员): 实现 */ (void)g; (void)teacherId; (void)result; (void)count; }
void     graph_getCoursesOfStudent(Graph* g, const char* studentId,
                                   char result[][MAX_ID_LEN], int* count)                { /* TODO(组员): 实现 */ (void)g; (void)studentId; (void)result; (void)count; }

double   graph_getEdgeWeight(Graph* g, const char* fromId,
                             const char* toId, EdgeType type)    { /* TODO(组员): 实现 */ (void)g; (void)fromId; (void)toId; (void)type; return 0.0; }
void     graph_print(Graph* g)                                   { /* TODO(组员): 实现 */ (void)g; }
