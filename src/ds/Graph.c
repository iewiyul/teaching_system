/*
负责人：陆奕炜
*/
#include "Graph.h"

Graph *graph_create(void) {
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    if (!graph)
        return NULL;
    memset(graph, 0, sizeof(Graph));
    return graph;
}

void graph_destroy(Graph *g) {
    if (NULL == g)
        return;
    for (int i = 0; i < MAX_VERTEX; i++) {
        AdjNode *node = g->adj[i], *temp;
        while (NULL != node) {
            temp = node;
            node = node->next;
            free(temp);
        }
    }
    free(g);
}

bool graph_addVertex(Graph *g, const char *id) {
    if (g->vertexCount == MAX_VERTEX)
        return false;
    for (int i = 0; i < g->vertexCount; i++) {
        if (0 == strcmp(id, g->vertexIds[i])) {
            return false;
        }
    }
    strncpy(g->vertexIds[g->vertexCount], id, MAX_ID_LEN - 1);
    g->vertexIds[g->vertexCount][MAX_ID_LEN - 1] = '\0';
    g->vertexCount++;
    return true;
}

static int findVertexIndex(const Graph *g, const char *id) {
    for (int i = 0; i < g->vertexCount; i++)
        if (strcmp(g->vertexIds[i], id) == 0)
            return i;
    return -1;
}

bool graph_addEdge(Graph *g, const char *fromId, const char *toId,
                   EdgeType type, double weight) {
    if (!g || !fromId || !toId)
        return false;
    // 判断顶点是否存在
    int f1, t1;
    if (-1 == (f1 = findVertexIndex(g, fromId))) {
        if (!graph_addVertex(g, fromId))
            return false;
        f1 = g->vertexCount - 1;
    }
    if (-1 == (t1 = findVertexIndex(g, toId))) {
        if (!graph_addVertex(g, toId))
            return false;
        t1 = g->vertexCount - 1;
    }
    // 判断是否有重复的边
    AdjNode *node = g->adj[f1], *temp = node;
    while (temp) {
        if (temp->type == type && !strcmp(temp->vertexId, toId) &&
            temp->weight == weight)
            return false;
        temp = temp->next;
    }
    // 添加边
    AdjNode *newNode = (AdjNode *)malloc(sizeof(AdjNode));
    if (!newNode)
        return false;
    memset(newNode, 0, sizeof(AdjNode));
    newNode->type = type;
    strncpy(newNode->vertexId, toId, MAX_ID_LEN - 1);
    newNode->vertexId[MAX_ID_LEN - 1] = '\0';
    newNode->weight = weight;
    if (!node) {
        g->adj[f1] = newNode;
    } else {
        newNode->next = node->next;
        node->next = newNode;
    }
    g->edgeCount++;
    return true;
}

bool graph_removeEdge(Graph *g, const char *fromId, const char *toId,
                      EdgeType type) {
    if (!g || !fromId || !toId)
        return false;
    // 判断顶点是否存在
    int f1;
    if (-1 == (f1 = findVertexIndex(g, fromId))) {
        return false;
    }

    AdjNode *init = g->adj[f1], *pre = NULL;
    while (init) {
        if (0 == strcmp(init->vertexId, toId) && init->type == type) {
            break;
        }
        pre = init;
        init = init->next;
    }
    if (!init)
        return false;
    else {
        AdjNode *temp = init;
        if (NULL == pre) {
            g->adj[f1] = init->next;
        } else {
            pre->next = init->next;
        }
        free(temp);
        g->edgeCount--;
    }
    return true;
}

bool graph_removeVertex(Graph *g, const char *id) {
    if (!g || !id)
        return false;
    int f1;
    if (-1 == (f1 = findVertexIndex(g, id))) {
        return false;
    }

    AdjNode *node = g->adj[f1], *temp;
    while (node) {
        temp = node;
        node = node->next;
        graph_removeEdge(g, id, temp->vertexId, temp->type);
    }
    // 删除入边
    for (int i = 0; i < g->vertexCount; i++) {
        if (i == f1) {
            continue;
        }
        AdjNode *node = g->adj[i], *temp;
        while (node) {
            if (0 == strcmp(node->vertexId, id)) {
                temp = node;
                node = node->next;
                graph_removeEdge(g, g->vertexIds[i], id, temp->type);
            } else {
                node = node->next;
            }
        }
    }

    for (int i = f1; i < g->vertexCount - 1; i++) {
        strcpy(g->vertexIds[i], g->vertexIds[i + 1]);
        g->adj[i] = g->adj[i + 1];
    }
    memset(g->vertexIds[g->vertexCount - 1], 0, MAX_ID_LEN);
    g->adj[g->vertexCount - 1] = NULL;
    g->vertexCount--;
    return true;
}

void graph_getNeighbors(Graph *g, const char *id, char result[][MAX_ID_LEN],
                        int *count) {
    if (!g || !id || !count) {
        return;
    }
    int f1 = findVertexIndex(g, id), num = 0;
    if (-1 == f1) {
        return;
    }
    AdjNode *init = g->adj[f1];
    while (init) {
        strcpy(result[num], init->vertexId);
        num++;
        init = init->next;
    }
    *count = num;
}
void graph_getTeachersOfCourse(Graph *g, const char *courseId,
                               char result[][MAX_ID_LEN], int *count) {
    if (!g || !courseId || !count) {
        return;
    }
    int f1, num = 0;
    if (-1 == (f1 = findVertexIndex(g, courseId))) {
        return;
    }
    for (int i = 0; i < g->vertexCount; i++) {
        if (f1 == i) {
            continue;
        }
        AdjNode *node = g->adj[i];
        while (node) {
            if (0 == strcmp(courseId, node->vertexId) &&
                node->type == EDGE_TEACH) {
                strcpy(result[num], g->vertexIds[i]);
                num++;
            }
            node = node->next;
        }
    }
    *count = num;
}

void graph_getStudentsOfCourse(Graph *g, const char *courseId,
                               char result[][MAX_ID_LEN], int *count) {
    if (!g || !courseId || !count) {
        return;
    }
    int f1, num = 0;
    if (-1 == (f1 = findVertexIndex(g, courseId))) {
        return;
    }
    for (int i = 0; i < g->vertexCount; i++) {
        if (f1 == i) {
            continue;
        }
        AdjNode *node = g->adj[i];
        while (node) {
            if (0 == strcmp(courseId, node->vertexId) &&
                node->type == EDGE_TAKE) {
                strcpy(result[num], g->vertexIds[i]);
                num++;
            }
            node = node->next;
        }
    }
    *count = num;
}
void graph_getCoursesOfTeacher(Graph *g, const char *teacherId,
                               char result[][MAX_ID_LEN], int *count) {
    if (!g || !teacherId || !count) {
        return;
    }
    int f1, num = 0;
    if (-1 == (f1 = findVertexIndex(g, teacherId))) {
        return;
    }
    AdjNode *node = g->adj[f1];
    while (node) {
        if (node->type == EDGE_TEACH) {
            strcpy(result[num], node->vertexId);
            num++;
        }
        node = node->next;
    }
    *count = num;
}
void graph_getCoursesOfStudent(Graph *g, const char *studentId,
                               char result[][MAX_ID_LEN], int *count) {
    if (!g || !studentId || !count) {
        return;
    }
    int f1, num = 0;
    if (-1 == (f1 = findVertexIndex(g, studentId))) {
        return;
    }
    AdjNode *node = g->adj[f1];
    while (node) {
        if (node->type == EDGE_TAKE) {
            strcpy(result[num], node->vertexId);
            num++;
        }
        node = node->next;
    }
    *count = num;
}

double graph_getEdgeWeight(Graph *g, const char *fromId, const char *toId,
                           EdgeType type) {
    if (!g || !fromId || !toId) {
        return 0.0;
    }
    int f1;
    if (-1 == (f1 = findVertexIndex(g, fromId))) {
        return 0.0;
    }
    AdjNode *node = g->adj[f1];
    while (node) {
        if (0 == strcmp(node->vertexId, toId) && node->type == type) {
            return node->weight;
        }
        node = node->next;
    }
    return 0.0;
}

void graph_print(Graph *g) {
    if (!g)
        return;
    for (int i = 0; i < g->vertexCount; i++) {
        AdjNode *node = g->adj[i];
        printf("第%d个顶点的编号: %s\n", i, g->vertexIds[i]);
        printf("与该顶点相连的边有:\n");
        while (node) {
            printf(
                "相连的顶点编号:%s    相连的顶点类型:%d   相连的顶点权重%f\n",
                node->vertexId, node->type, node->weight);
            node = node->next;
        }
    }
}
