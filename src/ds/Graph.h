/*
图（邻接表）：存储 Edge（教师/学生↔课程的 TEACH/TAKE 关系）
负责人：陆奕炜
*/
#ifndef GRAPH_H
#define GRAPH_H

#include "../common.h"

typedef enum {
    EDGE_TEACH,   /* 教师 → 课程（任课关系） */
    EDGE_TAKE     /* 学生 → 课程（选课关系） */
} EdgeType;

typedef struct {
    char    fromId[MAX_ID_LEN];
    char    toId[MAX_ID_LEN];
    EdgeType type;
    double   weight;
} Edge;

typedef struct AdjNode {
    char           vertexId[MAX_ID_LEN];
    EdgeType       type;
    double         weight;
    struct AdjNode* next;
} AdjNode;

typedef struct {
    char    vertexIds[MAX_VERTEX][MAX_ID_LEN];
    AdjNode* adj[MAX_VERTEX];
    int     vertexCount;
    int     edgeCount;
} Graph;

/* 创建空 Graph；vertexCount=edgeCount=0 */
Graph*   graph_create(void);
/* 销毁并释放所有邻接节点 */
void     graph_destroy(Graph* g);

/* 添加顶点；id 已存在则返回 false */
bool     graph_addVertex(Graph* g, const char* id);
/* 添加边（fromId → toId）；顶点不存在则先建顶点；同 type 重复边返回 false */
bool     graph_addEdge(Graph* g, const char* fromId, const char* toId,
                       EdgeType type, double weight);
/* 删除指定 type 的边；不存在返回 false */
bool     graph_removeEdge(Graph* g, const char* fromId, const char* toId, EdgeType type);
/* 删除顶点及其所有相关边；顶点不存在返回 false */
bool     graph_removeVertex(Graph* g, const char* id);

/* 取顶点 id 的所有邻居 id；写入 result 数组，*count 为邻居数 */
void     graph_getNeighbors(Graph* g, const char* id,
                            char result[][MAX_ID_LEN], int* count);
/* 取课程的所有任课教师（来自指向 courseId 的 EDGE_TEACH 边） */
void     graph_getTeachersOfCourse(Graph* g, const char* courseId,
                                   char result[][MAX_ID_LEN], int* count);
/* 取课程的所有选课学生（来自指向 courseId 的 EDGE_TAKE 边） */
void     graph_getStudentsOfCourse(Graph* g, const char* courseId,
                                   char result[][MAX_ID_LEN], int* count);
/* 取教师的所有任课课程（teacherId 发出的 EDGE_TEACH 边终点） */
void     graph_getCoursesOfTeacher(Graph* g, const char* teacherId,
                                   char result[][MAX_ID_LEN], int* count);
/* 取学生的所有选课课程（studentId 发出的 EDGE_TAKE 边终点） */
void     graph_getCoursesOfStudent(Graph* g, const char* studentId,
                                   char result[][MAX_ID_LEN], int* count);

/* 取指定边的权重；边不存在返回 0.0 */
double   graph_getEdgeWeight(Graph* g, const char* fromId,
                             const char* toId, EdgeType type);

/* 调试：打印所有顶点与邻接表 */
void     graph_print(Graph* g);

#endif /* GRAPH_H */