# 智慧教学管理软件系统 - 项目说明

> 上海大学《数据结构与算法基础》课程项目 · 2026-2027 学年

---

## 一、项目概述

**题目：** 智慧教学管理软件系统

**目标：** 实现一个智慧教学管理软件，覆盖教师、学生、课程、成绩、通知、排行榜等场景，重点体现**数据结构**的应用。

**技术栈：** C 语言（C11 标准），纯控制台交互，无第三方依赖。

**评分维度：** 功能实现 / 性能指标 / 工程规范 / 理论水平 / 团队分工。

---

## 二、目录结构

```
数据结构小项目/
├── README.md                         # 本文件
├── 课程项目26-27.docx                # 原始任务文档
├── docs/
│   └── 课程项目实施报告.docx          # 待撰写
└── src/
    ├── main.c                        # 程序入口
    ├── common.h                      # 公共类型/宏定义
    ├── entity/                       # 实体（纯结构体）
    │   ├── Teacher.h
    │   ├── Student.h
    │   ├── Course.h
    │   ├── Score.h
    │   ├── Notice.h
    │   └── Record.h
    ├── ds/                           # 数据结构（每个用具体类型实现）
    │   ├── SeqList.h/.c              # 顺序表（存 Teacher）
    │   ├── LinkedList.h/.c           # 链表（存 Student）
    │   ├── Stack.h/.c                # 栈（存 Record）
    │   ├── Queue.h/.c                # 队列（存 Notice）
    │   ├── HashTable.h/.c            # 哈希表（key=学号，value=Student*）
    │   ├── BST.h/.c                  # 二叉排序树（key=GPA，存 Student）
    │   ├── Heap.h/.c                 # 堆（存 Student）
    │   └── Graph.h/.c                # 图（存 Edge）
    ├── service/                      # 业务逻辑
    │   ├── TeacherService.h/.c
    │   ├── StudentService.h/.c
    │   ├── CourseService.h/.c
    │   ├── ScoreService.h/.c
    │   ├── NoticeService.h/.c
    │   ├── SearchService.h/.c
    │   ├── RankingService.h/.c
    │   └── OperationHistory.h/.c
    ├── ui/
    │   └── Menu.h/.c                 # 控制台菜单
    └── data/
        └── seed.c                    # 种子数据生成
```

---

## 三、团队分工总览

| 角色 | 数据结构 | 业务模块 | 报告章节 |
|------|----------|----------|----------|
| **陆奕炜** | 图 Graph | 主入口 / CourseService / 种子数据 / 报告统筹 | §1 §2.1 §3.3 §6 |
| **组员 B** | 栈 + 队列 | NoticeService / OperationHistory | §3.2 |
| **组员 C** | 顺序表 + 链表 | TeacherService / StudentService | §3.1 |
| **组员 D** | BST + 哈希表 | SearchService | §3.4 §4 |
| **组员 E** | 堆 | ScoreService / RankingService | §3.5 §4 |

---

## 四、C 语言约定

### 4.1 数据结构实现原则

**每个数据结构直接用具体业务类型实现，不用 `void*`、不用泛型。**

| 数据结构 | 内部存储类型 | 用于 |
|----------|-------------|------|
| 顺序表 SeqList | `Teacher` | 教师存储（也可复制一份用于 Course / Score）|
| 链表 LinkedList | `Student` | 学生存储 |
| 栈 Stack | `Record` | 操作历史 |
| 队列 Queue | `Notice` | 待发通知 |
| 哈希表 HashTable | key=`char[]`, value=`Student*` | 按学号精确查找 |
| 二叉排序树 BST | key=`double`, value=`Student` | 按绩点区间查询 |
| 堆 Heap | `Student` | TopK 排行榜 |
| 图 Graph | `Edge`（教师/学生-课程关联） | 课程关系 |

这样做的好处：类型安全、代码易读、调试方便、教学价值高。

### 4.2 命名规范

| 类型 | 命名 | 示例 |
|------|------|------|
| 结构体类型 | `PascalCase` | `Teacher`、`SeqList` |
| 结构体变量 | `s_xxx` 或 `xxx_` | `s_teachers`、`head_` |
| 全局变量 | `g_xxx` | `g_teacherService` |
| 函数 | `module_action` | `teacher_add`、`seqlist_insert` |
| 常量/宏 | `UPPER_SNAKE` | `MAX_STUDENTS` |

### 4.3 公共头文件 common.h

```c
#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define MAX_ID_LEN     20
#define MAX_NAME_LEN   50
#define MAX_TEXT_LEN   256

#endif
```

---

## 五、实体定义（entity/）—— 全员参考

所有实体由陆奕炜 统一定义，**纯结构体，无函数**。

### Teacher.h
```c
typedef struct {
    char id[MAX_ID_LEN];       // 工号，如 "T001"
    char name[MAX_NAME_LEN];   // 姓名
    char title[20];            // 职称（讲师/副教授/教授）
    char department[50];       // 院系
    double rating;             // 授课评分 0~5
} Teacher;
```

### Student.h
```c
typedef struct {
    char id[MAX_ID_LEN];       // 学号，如 "S001"
    char name[MAX_NAME_LEN];   // 姓名
    char gender[4];            // 性别
    char major[50];            // 专业
    int grade;                 // 年级
    double gpa;                // 绩点 0~5
    bool hasBadRecord;         // 是否有不良记录
    int badLevel;              // 不良记录严重程度 0~3
} Student;
```

### Course.h
```c
typedef struct {
    char id[MAX_ID_LEN];       // 课程号，如 "C001"
    char name[MAX_NAME_LEN];   // 课程名
    double credit;             // 学分
    int hours;                 // 学时
    char semester[20];         // 学期
} Course;
```

### Score.h
```c
typedef struct {
    char studentId[MAX_ID_LEN];
    char courseId[MAX_ID_LEN];
    double value;              // 分数 0~100
    char examType[20];         // 期中/期末/平时
} Score;
```

### Notice.h
```c
typedef struct {
    char id[MAX_ID_LEN];
    char title[100];
    char content[MAX_TEXT_LEN];
    char channel[10];          // "SMS"/"EMAIL"/"APP"
    char target[MAX_ID_LEN];
    bool sent;
} Notice;
```

### Record.h
```c
typedef struct {
    char opType[10];           // "add"/"remove"/"update"
    char entityType[20];       // "Teacher"/"Student"/...
    char targetId[MAX_ID_LEN];
    char description[100];
    long timestamp;
} Record;
```

---

## 六、陆奕炜 —— 图 Graph + 主入口 + CourseService

### 6.1 负责的文件

| 文件 | 说明 |
|------|------|
| `src/main.c` | 程序入口 |
| `src/entity/*.h` | 6 个实体头文件 |
| `src/common.h` | 公共宏 |
| `src/ds/Graph.h/.c` | 图（邻接表） |
| `src/service/CourseService.h/.c` | 课程业务 |
| `src/data/seed.c` | 种子数据生成（全员共用的基准数据） |
| `src/ui/Menu.h/.c` | 控制台**主菜单**分发器（`menu_run`，只负责调各 `xxx_service_menu`；各模块菜单由 B/C/D/E 在自己 service.c 里实现）|

### 6.2 Graph.h/.c 写法

```c
// Graph.h
#ifndef GRAPH_H
#define GRAPH_H

#include "common.h"

#define MAX_VERTEX 100

typedef enum { EDGE_TEACH, EDGE_TAKE } EdgeType;

typedef struct {
    char fromId[MAX_ID_LEN];
    char toId[MAX_ID_LEN];
    EdgeType type;
    double weight;
} Edge;

typedef struct AdjNode {
    char vertexId[MAX_ID_LEN];
    EdgeType type;
    double weight;
    struct AdjNode* next;
} AdjNode;

typedef struct {
    char vertexIds[MAX_VERTEX][MAX_ID_LEN];
    AdjNode* adj[MAX_VERTEX];
    int vertexCount;
    int edgeCount;
} Graph;

Graph* graph_create(void);
// 释放图内存（含所有邻接链表节点）
void    graph_destroy(Graph* g);

// 添加顶点；id 已存在返回 false
bool graph_addVertex(Graph* g, const char* id);
// 加一条 fromId→toId 的有向边，带 type/weight；两端顶点不存在时自动补建
bool graph_addEdge(Graph* g, const char* fromId, const char* toId,
                   EdgeType type, double weight);
// 删除指定类型的边（type 区分 EDGE_TEACH / EDGE_TAKE 同名边）；未找到返回 false
bool graph_removeEdge(Graph* g, const char* fromId, const char* toId, EdgeType type);
// 删除顶点及其所有邻接边（§6.3 功能 2 删除课程时调用）
bool graph_removeVertex(Graph* g, const char* id);

// 取顶点所有邻居 id（不限方向、不限类型）；result 需预分配至少 MAX_VERTEX 行
void graph_getNeighbors(Graph* g, const char* id,
                        char result[][MAX_ID_LEN], int* count);
// 取课程的所有任课教师（follow EDGE_TEACH 出边）
void graph_getTeachersOfCourse(Graph* g, const char* courseId,
                               char result[][MAX_ID_LEN], int* count);
// 取课程的所有选课学生（follow EDGE_TAKE 出边）
void graph_getStudentsOfCourse(Graph* g, const char* courseId,
                               char result[][MAX_ID_LEN], int* count);
// 取教师的所有任课课程（follow EDGE_TEACH 入边）
void graph_getCoursesOfTeacher(Graph* g, const char* teacherId,
                               char result[][MAX_ID_LEN], int* count);
// 取学生的所有选课课程（follow EDGE_TAKE 入边）
void graph_getCoursesOfStudent(Graph* g, const char* studentId,
                               char result[][MAX_ID_LEN], int* count);
// 取指定边的权重（§6.3 功能 8/9 用）；边不存在时返回 0.0
double graph_getEdgeWeight(Graph* g, const char* fromId,
                           const char* toId, EdgeType type);
// 打印全图（调试用）
void graph_print(Graph* g);

#endif
```

### 6.3 CourseService 业务功能

下面每条功能先给出 I/O 示例，紧接着是该功能的实现函数。**所有函数都定义在 `src/service/CourseService.c`**，文件顶部需要以下依赖：

```c
#include "service/CourseService.h"
#include "operation_history.h"   // B 提供的 history_record
#include <string.h>
```

为避免在每个功能里重复，下面的 `course_indexOf` 是文件内的私有辅助函数，其它函数都会调用它：

```c
// 根据 id 在 CourseList 中查找下标，找不到返回 -1
static int course_indexOf(CourseService* s, const char* id) {
    for (int i = 0; i < courselist_size(s->courses); i++) {
        Course* c = courselist_at(s->courses, i);
        if (strcmp(c->id, id) == 0) return i;
    }
    return -1;
}
```

#### 菜单

```
========== 课程管理 ==========
 1. 新增课程
 2. 删除课程
 3. 修改课程
 4. 按课程号查询
 5. 列出全部课程
 6. 添加任课教师
 7. 删除任课教师
 8. 查询某教师的所有课程
 9. 查询某课程的所有教师
10. 添加学生选课
11. 删除学生选课
12. 查询某学生的所有课程
13. 查询某课程的所有学生
 0. 返回主菜单
==============================
```

#### 【功能 1】新增课程

**输入（用户键入）：**
```
请输入选项: 1
--- 新增课程 ---
课程号: C006
课程名: 人工智能导论
学分: 3
学时: 48
学期: 2025秋
```

**输出（程序打印）：**
```
✅ 课程添加成功！
```

**实现：**
```c
bool course_add(CourseService* s, Course c) {
    if (course_indexOf(s, c.id) >= 0) {
        printf("❌ 课程号已存在\n");
        return false;
    }
    courselist_insert(s->courses, c);
    graph_addVertex(s->courseGraph, c.id);   // 课程也作为图中的顶点
    history_record("add", "Course", c.id, "新增课程");
    printf("✅ 课程添加成功！\n");
    return true;
}
```

#### 【功能 2】删除课程

**输入：**
```
请输入选项: 2
请输入要删除的课程号: C006
确认删除 C006 人工智能导论 吗？(y/n): y
```

**输出：**
```
✅ 课程已删除（同时清除该课程的所有任课/选课关系）
```

**实现：**
```c
bool course_remove(CourseService* s, const char* id) {
    int idx = course_indexOf(s, id);
    if (idx < 0) {
        printf("❌ 未找到该课程号\n");
        return false;
    }
    Course* c = courselist_at(s->courses, idx);

    char confirm;
    printf("确认删除 %s %s 吗？(y/n): ", c->id, c->name);
    scanf(" %c", &confirm);
    if (confirm != 'y' && confirm != 'Y') {
        printf("已取消\n");
        return false;
    }

    // 先清理图中所有与该课程相关的边（双向 + 两种类型都尝试一次）
    char nbr[200][MAX_ID_LEN];
    int n = 0;
    graph_getNeighbors(s->courseGraph, id, nbr, &n);
    for (int i = 0; i < n; i++) {
        graph_removeEdge(s->courseGraph, nbr[i], id, EDGE_TEACH);
        graph_removeEdge(s->courseGraph, nbr[i], id, EDGE_TAKE);
        graph_removeEdge(s->courseGraph, id, nbr[i], EDGE_TEACH);
        graph_removeEdge(s->courseGraph, id, nbr[i], EDGE_TAKE);
    }

    courselist_remove(s->courses, id);
    graph_removeVertex(s->courseGraph, id);   // 见 6.4.3 扩展
    history_record("remove", "Course", id, "删除课程");
    printf("✅ 课程已删除（同时清除该课程的所有任课/选课关系）\n");
    return true;
}
```

#### 【功能 3】修改课程

**输入：**
```
请输入选项: 3
请输入要修改的课程号: C006
新课程名: 人工智能导论（修订版）
新学分: 3.5
新学时: 56
新学期: 2025秋
```

**输出：**
```
✅ 课程修改成功！
```

**实现：**
```c
bool course_update(CourseService* s, const char* id, Course c) {
    if (!courselist_update(s->courses, id, c)) {
        printf("❌ 未找到该课程号\n");
        return false;
    }
    history_record("update", "Course", id, "修改课程");
    printf("✅ 课程修改成功！\n");
    return true;
}
```

#### 【功能 4】按课程号查询

**输入：**
```
请输入选项: 4
请输入课程号: C003
```

**输出：**
```
--- 查询结果 ---
课程号：C003
课程名：数据库原理
学分：4.0
学时：64
学期：2024秋
任课教师(3人)：T101, T102, T103
选课学生(78人)：S2023001, S2023002, ..., S2023078
```

**实现：**
```c
Course* course_queryById(CourseService* s, const char* id) {
    Course* c = courselist_find(s->courses, id);
    if (!c) {
        printf("❌ 未找到该课程号\n");
        return NULL;
    }
    printf("--- 查询结果 ---\n");
    printf("课程号：%s\n", c->id);
    printf("课程名：%s\n", c->name);
    printf("学分：%.1f\n", c->credit);
    printf("学时：%d\n",  c->hours);
    printf("学期：%s\n",  c->semester);

    // 任课教师
    char tch[100][MAX_ID_LEN];
    int tc = 0;
    graph_getTeachersOfCourse(s->courseGraph, id, tch, &tc);
    printf("任课教师(%d人)：", tc);
    for (int i = 0; i < tc; i++) {
        printf("%s%s", tch[i], (i == tc - 1) ? "" : ", ");
    }
    printf("\n");

    // 选课学生（仅前 10 条 + "..."）
    char stu[500][MAX_ID_LEN];
    int sc = 0;
    graph_getStudentsOfCourse(s->courseGraph, id, stu, &sc);
    printf("选课学生(%d人)：", sc);
    if (sc > 0) {
        int show = sc < 10 ? sc : 10;
        for (int i = 0; i < show; i++) {
            if (i > 0) printf(", ");
            printf("%s", stu[i]);
        }
        if (sc > show) printf(", ...");
    }
    printf("\n");

    return c;
}
```

#### 【功能 5】列出全部课程

**输入：**
```
请输入选项: 5
```

**输出：**
```
--- 全部课程（共 6 门）---
课程号   课程名           学分  学时  学期
C001     数据结构         4.0   64    2024秋
C002     算法分析         3.0   48    2024秋
C003     数据库原理       4.0   64    2024秋
C004     操作系统         4.0   64    2025春
C005     计算机网络       3.0   48    2025春
C006     人工智能导论     3.0   48    2025秋
```

**实现：**
```c
void course_listAll(CourseService* s) {
    int n = courselist_size(s->courses);
    printf("--- 全部课程（共 %d 门）---\n", n);
    printf("课程号   课程名           学分  学时  学期\n");
    for (int i = 0; i < n; i++) {
        Course* c = courselist_at(s->courses, i);
        printf("%-8s %-15s %-4.1f %-4d  %s\n",
               c->id, c->name, c->credit, c->hours, c->semester);
    }
}
```

#### 【功能 6】添加任课教师（图的 addEdge）

**输入：**
```
请输入选项: 6
--- 添加任课教师 ---
课程号: C006
教师工号: T105
授课评分(0~5): 4.5
```

**输出：**
```
✅ 任课关系已建立：C006 ← T105（评分 4.5）
```

**实现：**
```c
bool course_assignTeacher(CourseService* s, const char* courseId,
                          const char* teacherId, double rating) {
    if (course_indexOf(s, courseId) < 0) {
        printf("❌ 课程号不存在\n");
        return false;
    }
    if (!seqlist_find(s->teachersRef, teacherId)) {
        printf("❌ 教师工号不存在\n");
        return false;
    }
    // 边方向：teacher -> course；权重 = 授课评分
    graph_addEdge(s->courseGraph, teacherId, courseId, EDGE_TEACH, rating);
    history_record("add", "Edge", courseId, "新增任课关系");
    printf("✅ 任课关系已建立：%s ← %s（评分 %.1f）\n",
           courseId, teacherId, rating);
    return true;
}
```

#### 【功能 7】删除任课教师

**输入：**
```
请输入选项: 7
请输入课程号: C006
请输入教师工号: T105
确认解除 C006 的 T105 任课关系？(y/n): y
```

**输出：**
```
✅ 已解除 T105 在 C006 的任课关系
```

**实现：**
```c
bool course_unassignTeacher(CourseService* s, const char* courseId,
                            const char* teacherId) {
    if (course_indexOf(s, courseId) < 0) {
        printf("❌ 课程号不存在\n");
        return false;
    }
    if (!seqlist_find(s->teachersRef, teacherId)) {
        printf("❌ 教师工号不存在\n");
        return false;
    }

    char confirm;
    printf("确认解除 %s 的 %s 任课关系？(y/n): ", courseId, teacherId);
    scanf(" %c", &confirm);
    if (confirm != 'y' && confirm != 'Y') {
        printf("已取消\n");
        return false;
    }

    if (!graph_removeEdge(s->courseGraph, teacherId, courseId, EDGE_TEACH)) {
        printf("❌ 该任课关系不存在\n");
        return false;
    }
    history_record("remove", "Edge", courseId, "删除任课关系");
    printf("✅ 已解除 %s 在 %s 的任课关系\n", teacherId, courseId);
    return true;
}
```

#### 【功能 8】查询某教师的所有课程（图遍历）

**输入：**
```
请输入选项: 8
请输入教师工号: T102
```

**输出：**
```
--- T102 任课信息 ---
姓名：王芳
授课评分均值：4.3
任课课程（2 门）：
  - C001 数据结构      （评分 4.5）
  - C003 数据库原理    （评分 4.2）
```

**实现：**
```c
void course_queryTeacherCourses(CourseService* s, const char* teacherId) {
    Teacher* t = seqlist_find(s->teachersRef, teacherId);
    if (!t) {
        printf("❌ 教师工号不存在\n");
        return;
    }

    char cids[100][MAX_ID_LEN];
    int n = 0;
    graph_getCoursesOfTeacher(s->courseGraph, teacherId, cids, &n);

    printf("--- %s 任课信息 ---\n", teacherId);
    printf("姓名：%s\n", t->name);

    double sum = 0;
    for (int i = 0; i < n; i++) {
        sum += graph_getEdgeWeight(s->courseGraph, teacherId, cids[i], EDGE_TEACH);
    }
    printf("授课评分均值：%.1f\n", n > 0 ? sum / n : 0);

    printf("任课课程（%d 门）：\n", n);
    for (int i = 0; i < n; i++) {
        Course* c = courselist_find(s->courses, cids[i]);
        if (!c) continue;
        double w = graph_getEdgeWeight(s->courseGraph, teacherId, cids[i], EDGE_TEACH);
        printf("  - %s %-12s （评分 %.1f）\n", c->id, c->name, w);
    }
}
```

#### 【功能 9】查询某课程的所有教师

**输入：**
```
请输入选项: 9
请输入课程号: C001
```

**输出：**
```
--- C001 数据结构 任课教师（3 人）---
  T101 张伟   教授     评分 4.5
  T102 王芳   副教授   评分 4.5
  T103 刘洋   讲师     评分 4.3
```

**实现：**
```c
void course_queryCourseTeachers(CourseService* s, const char* courseId) {
    Course* c = courselist_find(s->courses, courseId);
    if (!c) {
        printf("❌ 课程号不存在\n");
        return;
    }
    char tids[100][MAX_ID_LEN];
    int n = 0;
    graph_getTeachersOfCourse(s->courseGraph, courseId, tids, &n);

    printf("--- %s %s 任课教师（%d 人）---\n", c->id, c->name, n);
    for (int i = 0; i < n; i++) {
        Teacher* t = seqlist_find(s->teachersRef, tids[i]);
        if (!t) continue;
        double w = graph_getEdgeWeight(s->courseGraph, tids[i], courseId, EDGE_TEACH);
        printf("  %-8s %-6s %-6s 评分 %.1f\n",
               t->id, t->name, t->title, w);
    }
}
```

#### 【功能 10】添加学生选课

**输入：**
```
请输入选项: 10
--- 添加学生选课 ---
课程号: C006
学号: S2023010
```

**输出：**
```
✅ 选课成功：C006 ← S2023010
```

**实现：**
```c
bool course_addStudent(CourseService* s, const char* courseId,
                       const char* studentId) {
    if (course_indexOf(s, courseId) < 0) {
        printf("❌ 课程号不存在\n");
        return false;
    }
    if (!linkedlist_find(s->studentsRef, studentId)) {
        printf("❌ 学号不存在\n");
        return false;
    }
    // 边方向：student -> course；权重无意义
    graph_addEdge(s->courseGraph, studentId, courseId, EDGE_TAKE, 0);
    history_record("add", "Edge", courseId, "新增选课");
    printf("✅ 选课成功：%s ← %s\n", courseId, studentId);
    return true;
}
```

#### 【功能 11】删除学生选课

**输入：**
```
请输入选项: 11
请输入课程号: C006
请输入学号: S2023010
确认 C006 删除 S2023010 的选课？(y/n): y
```

**输出：**
```
✅ 已退课：C006 ← S2023010
```

**实现：**
```c
bool course_removeStudent(CourseService* s, const char* courseId,
                          const char* studentId) {
    if (course_indexOf(s, courseId) < 0) {
        printf("❌ 课程号不存在\n");
        return false;
    }
    if (!linkedlist_find(s->studentsRef, studentId)) {
        printf("❌ 学号不存在\n");
        return false;
    }

    char confirm;
    printf("确认 %s 删除 %s 的选课？(y/n): ", courseId, studentId);
    scanf(" %c", &confirm);
    if (confirm != 'y' && confirm != 'Y') {
        printf("已取消\n");
        return false;
    }

    if (!graph_removeEdge(s->courseGraph, studentId, courseId, EDGE_TAKE)) {
        printf("❌ 该选课关系不存在\n");
        return false;
    }
    history_record("remove", "Edge", courseId, "删除选课");
    printf("✅ 已退课：%s ← %s\n", courseId, studentId);
    return true;
}
```

#### 【功能 12】查询某学生的所有课程

**输入：**
```
请输入选项: 12
请输入学号: S2023010
```

**输出：**
```
--- S2023010 的课表 ---
姓名：赵一
已选课程（4 门）：
  - C001 数据结构      （成绩：85）
  - C002 算法分析      （成绩：92）
  - C003 数据库原理    （成绩：78）
  - C006 人工智能导论  （成绩：暂无）
```

**实现：**
```c
void course_queryStudentCourses(CourseService* s, const char* studentId) {
    Student* stu = linkedlist_find(s->studentsRef, studentId);
    if (!stu) {
        printf("❌ 学号不存在\n");
        return;
    }
    char cids[100][MAX_ID_LEN];
    int n = 0;
    graph_getCoursesOfStudent(s->courseGraph, studentId, cids, &n);

    printf("--- %s 的课表 ---\n", studentId);
    printf("姓名：%s\n", stu->name);
    printf("已选课程（%d 门）：\n", n);
    for (int i = 0; i < n; i++) {
        Course* c = courselist_find(s->courses, cids[i]);
        if (!c) continue;
        // §10.6 已暴露 scorelist_find：§6.3 直接用，不再依赖 ScoreService 额外 API
        Score* sc = scorelist_find(s->scoresRef, studentId, cids[i]);
        if (!sc) {
            printf("  - %s %-12s （成绩：暂无）\n", c->id, c->name);
        } else {
            printf("  - %s %-12s （成绩：%.0f）\n", c->id, c->name, sc->score);
        }
    }
}
```

#### 【功能 13】查询某课程的所有学生

**输入：**
```
请输入选项: 13
请输入课程号: C001
```

**输出：**
```
--- C001 数据结构 选课学生（共 78 人）---
学号        姓名    绩点
S2023001    张三    3.5
S2023002    李四    3.8
...
（仅显示前 10 条，输入 0 查看全部）
```

**实现：**
```c
void course_queryCourseStudents(CourseService* s, const char* courseId) {
    Course* c = courselist_find(s->courses, courseId);
    if (!c) {
        printf("❌ 课程号不存在\n");
        return;
    }
    char sids[500][MAX_ID_LEN];
    int n = 0;
    graph_getStudentsOfCourse(s->courseGraph, courseId, sids, &n);

    printf("--- %s %s 选课学生（共 %d 人）---\n", c->id, c->name, n);
    if (n == 0) return;

    printf("学号        姓名    绩点\n");
    int show = n < 10 ? n : 10;
    for (int i = 0; i < show; i++) {
        Student* stu = linkedlist_find(s->studentsRef, sids[i]);
        if (stu) {
            printf("%-10s %-6s %.1f\n", stu->id, stu->name, stu->gpa);
        }
    }
    if (n > show) {
        printf("（仅显示前 10 条，输入 0 查看全部）\n");
    }
}
```

### 6.4 跨服务接口（由组长定义签名）

跨服务的回调统一由组长在 `common.h` 中定接口签名，**调用方和实现方各自独立**——每个组员只需面向接口编程，不需要互相等模块。

#### 6.4.1 history_record —— 操作日志写入

签名在 `common.h` 中由组长定义；**函数体在 `src/common.c` 中实现**（避免各组员互相调用对方私有函数）。A/C/D/E 在各自的 `xxx_add` / `xxx_remove` / `xxx_update` 中调用一次。

```c
// common.h 中由组长定义（A 负责维护）
void history_record(const char* opType, const char* entityType,
                    const char* targetId, const char* desc);

// src/common.c 中实现（陆奕炜 维护，跨层公共工具）：
void history_record(const char* opType, const char* entityType,
                    const char* targetId, const char* desc) {
    if (!g_operationHistory || !g_operationHistory->stack) return;
    Record r;
    memset(&r, 0, sizeof(r));
    strncpy(r.opType,      opType,                   sizeof(r.opType)      - 1);
    strncpy(r.entityType,  entityType,               sizeof(r.entityType)  - 1);
    strncpy(r.targetId,    targetId ? targetId : "", sizeof(r.targetId)    - 1);
    strncpy(r.description, desc,                     sizeof(r.description) - 1);
    r.timestamp = time(NULL);
    stack_push(g_operationHistory->stack, r);
}

// 其他组员调用示例（A 在 TeacherService 里）：
bool teacher_add(TeacherService* s, Teacher t) {
    if (!seqlist_insert(s->teachers, t)) return false;
    history_record("add", "Teacher", t.id, "新增教师");
    return true;
}
```

> 这是目前唯一的跨服务回调。如果后续需要新增（比如 `audit_log`），统一追加到 §6.4，不要散落到各组员的接口里。

### 6.5 seed.c 写法（由组长编写种子数据）

组长统一写种子数据，让所有组员**面对同一份基准数据**开发/测试，避免「A 用 100 学生、B 用 350 学生」导致联合调试时索引不一致。

调用顺序由组长定（其他组员不要改）：**5 门课程 → 15 教师 → 350 学生 → 15 任课边 → ~1000+ 选课边 → `search_service_rebuildIndexes()`**。最后一步必须，否则 §9 SearchService 的功能 1-4 跑不通。

```c
#include <time.h>
#include "entity/Teacher.h"
#include "entity/Student.h"
#include "entity/Course.h"
#include "service/TeacherService.h"
#include "service/StudentService.h"
#include "service/CourseService.h"
#include "service/SearchService.h"

extern TeacherService* g_teacherService;
extern StudentService* g_studentService;
extern CourseService*  g_courseService;
extern SearchService*  g_searchService;

void init_seed_data(void) {
    srand((unsigned)time(NULL));

    printf("[种子数据加载中...]\n");

    // 1. 5 门课程
    Course courses[] = {
        {"C001", "数据结构",     4, 64, "2024秋"},
        {"C002", "算法分析",     3, 48, "2024秋"},
        {"C003", "数据库原理",   4, 64, "2024秋"},
        {"C004", "操作系统",     4, 64, "2025春"},
        {"C005", "计算机网络",   3, 48, "2025春"},
    };
    for (int i = 0; i < 5; i++) course_add(g_courseService, courses[i]);
    printf("  课程数：%d\n", 5);

    // 2. 15 名教师
    const char* tnames[] = {"张伟","李娜","王芳","刘洋","陈静",
                            "杨帆","周强","吴敏","徐磊","孙丽",
                            "马超","朱琳","胡军","林峰","何婷"};
    const char* titles[] = {"教授","副教授","讲师"};
    for (int i = 0; i < 15; i++) {
        Teacher t;
        snprintf(t.id, MAX_ID_LEN, "T%03d", 101 + i);
        strncpy(t.name, tnames[i], MAX_NAME_LEN);
        strncpy(t.title, titles[i % 3], 20);
        strncpy(t.department, "计算机学院", 50);
        t.rating = 3.0 + (rand() % 200) / 100.0;
        teacher_add(g_teacherService, t);
    }
    printf("  教师数：%d\n", 15);

    // 3. 350 名学生
    const char* snames[] = {"赵一","钱二","孙三","李四","周五",
                            "吴六","郑七","王八","冯九","陈十"};
    const char* majors[] = {"计算机","软件工程","人工智能","信息安全"};
    for (int i = 0; i < 350; i++) {
        Student s;
        snprintf(s.id, MAX_ID_LEN, "S%05d", 2023001 + i);
        strncpy(s.name, snames[i % 10], MAX_NAME_LEN);
        strncpy(s.gender, (i % 2) ? "男" : "女", 4);
        strncpy(s.major, majors[i % 4], 50);
        s.grade = 2023;
        s.gpa = 2.0 + (rand() % 300) / 100.0;
        s.hasBadRecord = (rand() % 10 == 0);
        s.badLevel = s.hasBadRecord ? (rand() % 3 + 1) : 0;
        student_add(g_studentService, s);
    }
    printf("  学生数：%d\n", 350);

    // 4. 任课关系
    for (int c = 0; c < 5; c++) {
        for (int t = 0; t < 3; t++) {
            char cid[MAX_ID_LEN], tid[MAX_ID_LEN];
            snprintf(cid, MAX_ID_LEN, "C00%d", c + 1);
            snprintf(tid, MAX_ID_LEN, "T%03d", 101 + c * 3 + t);
            course_assignTeacher(g_courseService, cid, tid,
                                 4.0 + (rand() % 100) / 100.0);
        }
    }
    printf("  建立任课关系：15 条\n");

    // 5. 选课关系
    int enrollCount = 0;
    for (int i = 0; i < 350; i++) {
        char sid[MAX_ID_LEN];
        snprintf(sid, MAX_ID_LEN, "S%05d", 2023001 + i);
        int n = 3 + rand() % 3;
        for (int j = 0; j < n; j++) {
            char cid[MAX_ID_LEN];
            snprintf(cid, MAX_ID_LEN, "C00%d", 1 + rand() % 5);
            if (course_addStudent(g_courseService, cid, sid)) {
                enrollCount++;
            }
        }
    }
    printf("  建立选课关系：约 %d 条\n", enrollCount);

    // 6. 重建查找索引
    printf("  构建查找索引...\n");
    search_service_rebuildIndexes(g_searchService);

    printf("[本地数据生成完成]\n\n");
}
```

### 6.6 CourseService 接口定义

CourseService 用一个并行的 `CourseList`（与 §8.2 的 `SeqList` 平行，存 `Course` 而非 `Teacher`）存课程基础信息；并持有对教师顺序表、学生链表、成绩服务的引用，用于在查询/打印时显示姓名与单科成绩。函数体见 §6.3。

```c
// ----- 课程容器 CourseList（与 SeqList 平行） -----
typedef struct {
    Course* data;
    int size;
    int capacity;
} CourseList;

CourseList* courselist_create(void);
void        courselist_destroy(CourseList* l);
bool        courselist_insert(CourseList* l, Course c);
bool        courselist_remove(CourseList* l, const char* id);
bool        courselist_update(CourseList* l, const char* id, Course c);
Course*     courselist_find(CourseList* l, const char* id);
Course*     courselist_at(CourseList* l, int index);
int         courselist_size(CourseList* l);

// ----- CourseService 主结构 -----
typedef struct {
    CourseList*   courses;        // 课程基础信息
    Graph*        courseGraph;    // 任课/选课关系图
    SeqList*      teachersRef;    // 引用全局教师顺序表（用于显示姓名）
    LinkedList*   studentsRef;    // 引用全局学生链表（用于显示姓名）
    ScoreService* scoresRef;      // 引用成绩服务（用于显示单科成绩）
} CourseService;

CourseService* course_service_create(SeqList* teachers,
                                     LinkedList* students,
                                     ScoreService* scores);
void course_service_destroy(CourseService* s);

// ----- 业务函数（实现见 §6.3）-----
bool course_add(CourseService* s, Course c);
bool course_remove(CourseService* s, const char* id);
bool course_update(CourseService* s, const char* id, Course c);
Course* course_queryById(CourseService* s, const char* id);
void course_listAll(CourseService* s);

bool course_assignTeacher(CourseService* s, const char* courseId,
                          const char* teacherId, double rating);
bool course_unassignTeacher(CourseService* s, const char* courseId,
                            const char* teacherId);
void course_queryTeacherCourses(CourseService* s, const char* teacherId);
void course_queryCourseTeachers(CourseService* s, const char* courseId);

bool course_addStudent(CourseService* s, const char* courseId,
                       const char* studentId);
bool course_removeStudent(CourseService* s, const char* courseId,
                          const char* studentId);
void course_queryStudentCourses(CourseService* s, const char* studentId);
void course_queryCourseStudents(CourseService* s, const char* courseId);

Graph*      course_service_getGraph(CourseService* s);
CourseList* course_service_getList(CourseService* s);
```

### 6.7 main.c 写法

main.c 只负责：① 定义全局 Service 句柄；② 在 `main()` 里 create → seed → `menu_run()` → destroy。每个 service 的 `xxx_service_menu` 由对应组员实现并通过头文件 include 进来。

```c
#include "service/TeacherService.h"
#include "service/StudentService.h"
#include "service/CourseService.h"
#include "service/ScoreService.h"
#include "service/SearchService.h"
#include "service/RankingService.h"
#include "service/NoticeService.h"
#include "service/OperationHistory.h"
#include "data/seed.c"
#include "ui/Menu.h"

TeacherService*   g_teacherService    = NULL;
StudentService*   g_studentService    = NULL;
CourseService*    g_courseService     = NULL;
ScoreService*     g_scoreService      = NULL;
NoticeService*    g_noticeService     = NULL;
SearchService*    g_searchService     = NULL;
RankingService*   g_rankingService    = NULL;
OperationHistory* g_operationHistory  = NULL;

int main(void) {
    g_teacherService    = teacher_service_create();
    g_studentService    = student_service_create();
    g_courseService     = course_service_create();
    g_scoreService      = score_service_create();
    g_noticeService     = notice_service_create();
    g_searchService     = search_service_create(g_studentService, g_teacherService);
    g_rankingService    = ranking_service_create(g_studentService, g_teacherService);
    g_operationHistory  = operation_history_create();

    init_seed_data();   // A 负责（§6.5）
    menu_run();         // 进入主菜单（§6.8）

    teacher_service_destroy(g_teacherService);
    // ... 释放其他 Service
    return 0;
}
```

### 6.8 Menu.c —— 主菜单分发器

组长写的 Menu.c **只**包含 `menu_run()`：列出 8 个模块入口，根据用户选择调对应组员实现的 `xxx_service_menu`。**不**含任何业务循环——那是 B/C/D/E 的活。

```c
#include "ui/Menu.h"
#include "service/TeacherService.h"
#include "service/StudentService.h"
#include "service/CourseService.h"
#include "service/ScoreService.h"
#include "service/SearchService.h"
#include "service/RankingService.h"
#include "service/NoticeService.h"
#include "service/OperationHistory.h"

extern TeacherService*   g_teacherService;
extern StudentService*   g_studentService;
extern CourseService*    g_courseService;
extern ScoreService*     g_scoreService;
extern SearchService*    g_searchService;
extern RankingService*   g_rankingService;
extern NoticeService*    g_noticeService;
extern OperationHistory* g_operationHistory;

void menu_run(void) {
    int choice;
    while (true) {
        printf("\n========== 智慧教学管理软件 ==========\n");
        printf(" 1. 教师管理\n");
        printf(" 2. 学生管理\n");
        printf(" 3. 课程管理\n");
        printf(" 4. 成绩管理\n");
        printf(" 5. 排行榜\n");
        printf(" 6. 通知管理\n");
        printf(" 7. 操作历史\n");
        printf(" 8. 查找服务\n");
        printf(" 0. 退出\n");
        printf("====================================\n");
        printf("请输入选项: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: teacher_service_menu(g_teacherService);      break;
            case 2: student_service_menu(g_studentService);      break;
            case 3: course_service_menu(g_courseService);        break;
            case 4: score_service_menu(g_scoreService);          break;
            case 5: ranking_service_menu(g_rankingService);      break;
            case 6: notice_service_menu(g_noticeService);        break;
            case 7: operation_history_menu(g_operationHistory);   break;
            case 8: search_service_menu(g_searchService);        break;
            case 0: return;
            default: printf("❌ 无效选项\n");
        }
    }
}
```

---

## 七、组员 B —— 栈 + 队列 + NoticeService + OperationHistory

### 7.1 负责的文件

| 文件 | 说明 |
|------|------|
| `src/ds/Stack.h/.c` | 栈（存 Record） |
| `src/ds/Queue.h/.c` | 队列（存 Notice） |
| `src/service/NoticeService.h/.c` | 通知推送 |
| `src/service/OperationHistory.h/.c` | 操作历史 |

### 7.2 Stack.h 写法（存 Record）

```c
typedef struct {
    Record* data;
    int top;
    int capacity;
} Stack;

// 创建空栈
Stack* stack_create(void);
// 释放栈内存
void   stack_destroy(Stack* s);

// 入栈；返回 false 表示栈已满
bool    stack_push(Stack* s, Record r);
// 出栈并返回栈顶 Record；栈空时返回全零 Record
Record  stack_pop(Stack* s);
// 查看栈顶 Record 但不出栈；栈空时返回全零 Record
Record  stack_peek(Stack* s);
// 栈是否为空（top == -1）
bool    stack_isEmpty(Stack* s);
// 当前栈中元素个数（top + 1）
int     stack_size(Stack* s);
// 清空栈（top = -1，容量不变）
void    stack_clear(Stack* s);
```

### 7.3 Queue.h 写法（存 Notice）

```c
typedef struct QueueNode {
    Notice data;
    struct QueueNode* next;
} QueueNode;

typedef struct {
    QueueNode* front;
    QueueNode* rear;
    int size;
} Queue;

// 创建空队列
Queue* queue_create(void);
// 释放队列内存（含所有节点）
void   queue_destroy(Queue* q);

// 队尾入队；返回 false 表示新节点分配失败
bool    enqueue(Queue* q, Notice n);
// 队头出队并返回 Notice；队列空时返回全零 Notice
Notice  dequeue(Queue* q);
// 查看队头 Notice 但不出队；队列空时返回全零 Notice
Notice  queue_peek(Queue* q);
// 队列是否为空（size == 0）
bool    queue_isEmpty(Queue* q);
// 当前队列中元素个数
int     queue_size(Queue* q);
// 按 [1] channel → target title 格式逐条打印所有 Notice
void    queue_traverse(Queue* q);
// 清空队列（释放所有节点，size = 0）
void    queue_clear(Queue* q);
```

### 7.4 NoticeService 业务功能（每个功能附 input/output）

通知队列用 §7.3 的 `Queue` 存 `Notice`。下面每条功能先给 I/O 再给实现，函数体在 `src/service/NoticeService.c`。

#### 菜单

```
========== 通知管理 ==========
 1. 新增通知（入队）
 2. 发送下一条通知（出队）
 3. 查看队首通知
 4. 列出所有待发通知
 5. 按通道过滤（SMS/EMAIL/APP）
 6. 清空通知队列
 0. 返回主菜单
==============================
```

#### 【功能 1】新增通知（入队）

**输入：**
```
请输入选项: 1
--- 新增通知 ---
接收者 ID: T102
标题: 关于下周三教研会议
内容: 上午10点在学院会议室召开...
通道(1.SMS 2.EMAIL 3.APP): 1
```

**输出：**
```
✅ 通知已加入队列（当前队列长度：1）
```

**实现：**
```c
bool notice_add(NoticeService* s, Notice n) {
    n.sent = false;
    enqueue(s->pending, n);
    printf("✅ 通知已加入队列（当前队列长度：%d）\n",
           queue_size(s->pending));
    history_record("add", "Notice", n.target, "新增通知");
    return true;
}
```

#### 【功能 2】发送下一条通知（出队，FIFO 演示）

**输入：**
```
请输入选项: 2
```

**输出：**
```
--- 发送下一条通知 ---
[队列出队] 取出第 1 条
📱 [SMS 已发送] → T102 王芳
  标题：关于下周三教研会议
  内容：上午10点在学院会议室召开...
✅ 发送完成！（队列剩余 2 条）
```

**实现：**
```c
bool notice_sendNext(NoticeService* s) {
    if (queue_isEmpty(s->pending)) {
        printf("❌ 队列为空，无通知可发送\n");
        return false;
    }
    Notice n = dequeue(s->pending);
    n.sent = true;
    printf("--- 发送下一条通知 ---\n[队列出队] 取出第 1 条\n");
    printf("📱 [%s 已发送] → %s\n", n.channel, n.target);
    printf("  标题：%s\n  内容：%s\n", n.title, n.content);
    printf("✅ 发送完成！（队列剩余 %d 条）\n",
           queue_size(s->pending));
    history_record("send", "Notice", n.target, "发送通知");
    return true;
}
```

> 输出中的「王芳」是教师姓名。要支持显示姓名，需给 `NoticeService` 加 `SeqList* teachersRef` 引用（与 §6.4 CourseService 同模式），本节先简化只打 target ID。

#### 【功能 3】查看队首通知

**输入：**
```
请输入选项: 3
```

**输出：**
```
[队首预览] SMS → T102
   标题：关于下周三教研会议
   内容：上午10点在学院会议室召开...
（不会出队，仍在队列中）
```

**实现：**
```c
void notice_peek(NoticeService* s) {
    if (queue_isEmpty(s->pending)) {
        printf("❌ 队列为空\n");
        return;
    }
    Notice n = queue_peek(s->pending);
    printf("[队首预览] %s → %s\n", n.channel, n.target);
    printf("   标题：%s\n   内容：%s\n", n.title, n.content);
    printf("（不会出队，仍在队列中）\n");
}
```

#### 【功能 4】列出所有待发通知

**输入：**
```
请输入选项: 4
```

**输出：**
```
--- 待发通知队列（共 3 条）---
[1] SMS    → T102      关于下周三教研会议
[2] EMAIL  → S2023010  关于期末考试安排
[3] APP    → ALL       关于五一放假通知
```

**实现：**
```c
void notice_listAll(NoticeService* s) {
    printf("--- 待发通知队列（共 %d 条）---\n",
           queue_size(s->pending));
    queue_traverse(s->pending);   // Queue.c 中按 [N] ... 格式打印
}
```

#### 【功能 5】按通道过滤

**输入：**
```
请输入选项: 5
请输入通道(SMS/EMAIL/APP): SMS
```

**输出：**
```
--- 通道为 SMS 的通知（共 1 条）---
[1] SMS    → T102      关于下周三教研会议
```

**实现：**
```c
void notice_filterByChannel(NoticeService* s, const char* channel) {
    printf("--- 通道为 %s 的通知 ---\n", channel);
    QueueNode* cur = s->pending->front;
    int idx = 0, hit = 0;
    while (cur) {
        if (strcmp(cur->data.channel, channel) == 0) {
            printf("[%d] %-6s → %-8s %s\n",
                   ++idx, cur->data.channel, cur->data.target,
                   cur->data.title);
            hit++;
        }
        cur = cur->next;
    }
    printf("（共 %d 条）\n", hit);
}
```

#### 【功能 6】清空队列

**输入：**
```
请输入选项: 6
确认清空所有待发通知？(y/n): y
```

**输出：**
```
✅ 队列已清空（原 3 条通知全部丢弃）
```

**实现：**
```c
bool notice_clear(NoticeService* s) {
    int n = queue_size(s->pending);
    char confirm;
    printf("确认清空所有待发通知？(y/n): ");
    scanf(" %c", &confirm);
    if (confirm != 'y' && confirm != 'Y') {
        printf("已取消\n");
        return false;
    }
    queue_clear(s->pending);
    printf("✅ 队列已清空（原 %d 条通知全部丢弃）\n", n);
    return true;
}
```

### 7.5 OperationHistory 业务功能

操作历史用 §7.2 的 `Stack` 存 `Record`。下面每条功能先给 I/O 再给实现，函数体在 `src/service/OperationHistory.c`。

#### 菜单

```
========== 操作历史 ==========
 1. 查看最近 N 次操作
 2. 撤销最近一次操作
 3. 清空历史
 0. 返回主菜单
==============================
```

#### 【功能 1】查看最近 N 次操作（栈顶 → 栈底）

**输入：**
```
请输入选项: 1
请输入 N: 5
```

**输出：**
```
--- 最近 5 次操作（栈顶 → 栈底）---
[1] 2026-09-22 14:32:15  add      Teacher     T116    新增教师张三
[2] 2026-09-22 14:33:02  remove   Student     S2023045 删除学生李四
[3] 2026-09-22 14:35:18  update   Course      C001    修改课程学分
[4] 2026-09-22 14:36:00  add      Notice      -       新增通知
[5] 2026-09-22 14:38:42  remove   Teacher     T110    删除教师孙七
```

**实现：** 用临时栈 pop 出来打印后再 push 回去，避免给 Stack 加 peek-at-index。

```c
void history_viewRecentN(OperationHistory* s, int n) {
    if (stack_isEmpty(s->stack)) {
        printf("❌ 暂无操作历史\n");
        return;
    }
    int total = stack_size(s->stack);
    int show = (n < total) ? n : total;
    printf("--- 最近 %d 次操作（栈顶 → 栈底）---\n", show);
    Stack* tmp = stack_create();
    for (int i = 0; i < show; i++) {
        Record r = stack_pop(s->stack);
        char buf[32]; format_time(r.timestamp, buf);
        const char* tid = strlen(r.targetId) ? r.targetId : "-";
        printf("[%d] %s  %-7s %-10s %-8s %s\n",
               i + 1, buf, r.opType, r.entityType, tid, r.description);
        stack_push(tmp, r);
    }
    while (!stack_isEmpty(tmp)) stack_push(s->stack, stack_pop(tmp));
    stack_destroy(tmp);
}
```

#### 【功能 2】撤销最近一次操作（栈弹出）

**输入：**
```
请输入选项: 2
```

**输出：**
```
--- 撤销最近一次操作 ---
[栈弹出] 取出栈顶
操作类型：remove
操作对象：Teacher T110
操作描述：删除教师孙七
是否继续撤销？(y/n): y
✅ 已记录撤销请求（请调用对应 Service 的恢复接口）
```

**实现：**
```c
void history_undoLast(OperationHistory* s) {
    if (stack_isEmpty(s->stack)) {
        printf("❌ 暂无操作可撤销\n");
        return;
    }
    Record r = stack_pop(s->stack);
    printf("--- 撤销最近一次操作 ---\n[栈弹出] 取出栈顶\n");
    printf("操作类型：%s\n", r.opType);
    printf("操作对象：%s %s\n", r.entityType, r.targetId);
    printf("操作描述：%s\n", r.description);
    char c; printf("是否继续撤销？(y/n): "); scanf(" %c", &c);
    if (c == 'y' || c == 'Y') history_undoLast(s);
    else printf("✅ 已记录撤销请求（请调用对应 Service 的恢复接口）\n");
}
```

#### 【功能 3】清空历史

**输入：**
```
请输入选项: 3
确认清空所有历史记录？(y/n): y
```

**输出：**
```
✅ 历史已清空
```

**实现：**
```c
void history_clear(OperationHistory* s) {
    char c; printf("确认清空所有历史记录？(y/n): "); scanf(" %c", &c);
    if (c != 'y' && c != 'Y') { printf("已取消\n"); return; }
    stack_clear(s->stack);
    printf("✅ 历史已清空\n");
}
```

### 7.6 菜单实现

B 在自己的 service.c 里写菜单循环，**不**走组长 Menu.c。每个 service 文件末尾追加一个 `xxx_service_menu` 函数，main.c 那边只 include 头文件并调用。

#### 7.6.1 notice_service_menu（在 NoticeService.c）

```c
void notice_service_menu(NoticeService* s) {
    int choice;
    while (true) {
        printf("\n========== 通知管理 ==========\n");
        printf(" 1. 新增通知（入队）\n");
        printf(" 2. 发送下一条通知（出队）\n");
        printf(" 3. 查看队首通知\n");
        printf(" 4. 列出所有待发通知\n");
        printf(" 5. 按通道过滤（SMS/EMAIL/APP）\n");
        printf(" 6. 清空通知队列\n");
        printf(" 0. 返回主菜单\n");
        printf("==============================\n");
        printf("请输入选项: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: {
                Notice n; memset(&n, 0, sizeof(n));
                printf("接收者 ID: "); scanf("%s", n.target);
                printf("标题: ");      scanf("%s", n.title);
                printf("内容: ");      scanf("%s", n.content);
                int ch; printf("通道(1.SMS 2.EMAIL 3.APP): "); scanf("%d", &ch);
                strcpy(n.channel, ch == 1 ? "SMS" : ch == 2 ? "EMAIL" : "APP");
                notice_add(s, n);
                break;
            }
            case 2: notice_sendNext(s);    break;
            case 3: notice_peek(s);        break;
            case 4: notice_listAll(s);     break;
            case 5: {
                char ch[10]; printf("请输入通道(SMS/EMAIL/APP): "); scanf("%s", ch);
                notice_filterByChannel(s, ch);
                break;
            }
            case 6: notice_clear(s);       break;
            case 0: return;
            default: printf("❌ 无效选项\n");
        }
    }
}
```

#### 7.6.2 operation_history_menu（在 OperationHistory.c）

```c
void operation_history_menu(OperationHistory* s) {
    int choice;
    while (true) {
        printf("\n========== 操作历史 ==========\n");
        printf(" 1. 查看最近 N 次操作\n");
        printf(" 2. 撤销最近一次操作\n");
        printf(" 3. 清空历史\n");
        printf(" 0. 返回主菜单\n");
        printf("==============================\n");
        printf("请输入选项: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: {
                int n; printf("请输入 N: "); scanf("%d", &n);
                history_viewRecentN(s, n);
                break;
            }
            case 2: history_undoLast(s); break;
            case 3: history_clear(s);    break;
            case 0: return;
            default: printf("❌ 无效选项\n");
        }
    }
}
```

### 7.7 接口定义

```c
// ----- NoticeService -----
typedef struct {
    Queue* pending;
} NoticeService;

NoticeService* notice_service_create(void);
void           notice_service_destroy(NoticeService* s);

bool   notice_add(NoticeService* s, Notice n);
bool   notice_sendNext(NoticeService* s);
void   notice_peek(NoticeService* s);
void   notice_listAll(NoticeService* s);
void   notice_filterByChannel(NoticeService* s, const char* channel);
bool   notice_clear(NoticeService* s);

void   notice_service_menu(NoticeService* s);   // 见 §7.6.1

// ----- OperationHistory -----
typedef struct {
    Stack* stack;
} OperationHistory;

OperationHistory* operation_history_create(void);
void              operation_history_destroy(OperationHistory* s);

void history_viewRecentN(OperationHistory* s, int n);
void history_undoLast(OperationHistory* s);
void history_clear(OperationHistory* s);

void operation_history_menu(OperationHistory* s);   // 见 §7.6.2
```

---

## 八、组员 C —— 顺序表 + 链表 + TeacherService + StudentService

### 8.1 负责的文件

| 文件 | 说明 |
|------|------|
| `src/ds/SeqList.h/.c` | 顺序表（存 Teacher） |
| `src/ds/LinkedList.h/.c` | 链表（存 Student） |
| `src/service/TeacherService.h/.c` | 教师服务 |
| `src/service/StudentService.h/.c` | 学生服务 |

### 8.2 SeqList 与 LinkedList 写法

顺序表用 `Teacher`、链表用 `Student`，与 §6.2 的图一样直接存具体业务类型。

```c
// ----- SeqList.h（存 Teacher） -----
typedef struct {
    Teacher* data;
    int size;
    int capacity;
} SeqList;

// 创建空顺序表
SeqList* seqlist_create(void);
// 释放顺序表内存（含 Teacher 数组）
void     seqlist_destroy(SeqList* s);

// 末尾追加 Teacher；容量不足自动扩容；返回 false 表示扩容失败
bool    seqlist_insert(SeqList* s, Teacher t);
// 按 id 删除首个匹配项；找到并删除返回 true，未找到返回 false
bool    seqlist_remove(SeqList* s, const char* id);
// 按 id 替换为新 Teacher；未找到返回 false
bool    seqlist_update(SeqList* s, const char* id, Teacher t);
// 按 id 查找；返回指向内部 Teacher 的指针（不要 free），未找到返回 NULL
Teacher* seqlist_find(SeqList* s, const char* id);
// 按下标取 Teacher 指针；越界返回 NULL
Teacher* seqlist_at(SeqList* s, int index);

// 按工号升序排序（strcmp）
void    seqlist_sort_by_id(SeqList* s);
// 按授课评分降序排序（§8.3 功能 6 用）
void    seqlist_sort_by_rating_desc(SeqList* s);

// 按表格格式遍历打印所有 Teacher（§8.3 功能 5/6 用）
void    seqlist_traverse(SeqList* s);
// 当前顺序表元素个数
int     seqlist_size(SeqList* s);
// 顺序表是否为空（size == 0）
bool    seqlist_isEmpty(SeqList* s);

// ----- LinkedList.h（存 Student） -----
typedef struct ListNode {
    Student data;
    struct ListNode* next;
} ListNode;

typedef struct {
    ListNode* head;
    int size;
} LinkedList;

// 创建空链表
LinkedList* linkedlist_create(void);
// 释放链表内存（含所有节点）
void        linkedlist_destroy(LinkedList* l);

// 在 head 处头插 Student；返回 false 表示节点分配失败
bool     linkedlist_insert(LinkedList* l, Student s);
// 在 tail 处尾插 Student；返回 false 表示节点分配失败
bool     linkedlist_insertTail(LinkedList* l, Student s);
// 按 id 删除首个匹配节点；找到并删除返回 true，未找到返回 false
bool     linkedlist_remove(LinkedList* l, const char* id);
// 按 id 查找；返回指向节点内 Student 的指针（不要 free），未找到返回 NULL
Student* linkedlist_find(LinkedList* l, const char* id);

// 按绩点降序排序（会破坏原链表顺序；§8.4 功能 6 复制一份再排）
void     linkedlist_sort_by_gpa_desc(LinkedList* l);

// 按 [N] 学号 姓名 ... 格式遍历打印所有 Student
void     linkedlist_traverse(LinkedList* l);
// 当前链表节点个数
int      linkedlist_size(LinkedList* l);
// 链表是否为空（head == NULL）
bool     linkedlist_isEmpty(LinkedList* l);
```

### 8.3 TeacherService 业务功能

教师用 §8.2 的 `SeqList` 存 `Teacher`。下面每条功能先给 I/O 再给实现，函数体在 `src/service/TeacherService.c`。

#### 菜单

```
========== 教师管理 ==========
 1. 新增教师
 2. 删除教师（按工号）
 3. 修改教师
 4. 按工号查询
 5. 列出全部教师
 6. 按授课评分排序
 0. 返回主菜单
==============================
```

#### 【功能 1】新增教师

**输入：**
```
请输入选项: 1
--- 新增教师 ---
工号: T116
姓名: 张三
职称: 讲师
院系: 计算机学院
授课评分: 4.0
```

**输出：**
```
✅ 教师添加成功！
```

**实现：**
```c
bool teacher_add(TeacherService* s, Teacher t) {
    if (!seqlist_insert(s->teachers, t)) return false;
    history_record("add", "Teacher", t.id, "新增教师");
    printf("✅ 教师添加成功！\n");
    return true;
}
```

#### 【功能 2】删除教师

**输入：**
```
请输入选项: 2
请输入工号: T116
```

**输出：**
```
✅ 教师删除成功！
```

或：
```
❌ 未找到该工号
```

**实现：**
```c
bool teacher_remove(TeacherService* s, const char* id) {
    if (!seqlist_remove(s->teachers, id)) {
        printf("❌ 未找到该工号\n");
        return false;
    }
    history_record("remove", "Teacher", id, "删除教师");
    printf("✅ 教师删除成功！\n");
    return true;
}
```

#### 【功能 3】修改教师

**输入：**
```
请输入选项: 3
请输入工号: T102
新姓名: 王芳芳
新职称: 教授
新院系: 计算机学院
新评分: 4.8
```

**输出：**
```
✅ 教师修改成功！
```

**实现：**
```c
bool teacher_update(TeacherService* s, const char* id, Teacher t) {
    if (!seqlist_update(s->teachers, id, t)) return false;
    history_record("update", "Teacher", id, "修改教师");
    printf("✅ 教师修改成功！\n");
    return true;
}
```

#### 【功能 4】按工号查询

**输入：**
```
请输入选项: 4
请输入工号: T102
```

**输出：**
```
--- 查询结果 ---
工号：T102
姓名：王芳
职称：副教授
院系：计算机学院
授课评分：4.3
```

**实现：**
```c
Teacher* teacher_queryById(TeacherService* s, const char* id) {
    Teacher* t = seqlist_find(s->teachers, id);
    if (!t) { printf("❌ 未找到该工号\n"); return NULL; }
    printf("--- 查询结果 ---\n");
    printf("工号：%s\n姓名：%s\n职称：%s\n院系：%s\n授课评分：%.1f\n",
           t->id, t->name, t->title, t->dept, t->rating);
    return t;
}
```

#### 【功能 5】列出全部教师

**输入：**
```
请输入选项: 5
```

**输出：**
```
--- 全部教师（共 16 人）---
工号   姓名   职称     院系        评分
T101   张伟   教授     计算机学院  4.8
T102   王芳   副教授   计算机学院  4.3
T103   刘洋   讲师     计算机学院  3.9
...
```

**实现：**
```c
void teacher_listAll(TeacherService* s) {
    printf("--- 全部教师（共 %d 人）---\n",
           seqlist_size(s->teachers));
    seqlist_traverse(s->teachers);   // SeqList.c 中按表格格式打印
}
```

#### 【功能 6】按授课评分排序

**输入：**
```
请输入选项: 6
```

**输出：**
```
--- 教师按评分降序 ---
T105   杨帆   教授     评分 4.9
T101   张伟   教授     评分 4.8
T108   吴敏   副教授   评分 4.7
T104   陈静   教授     评分 4.5
...
（排序耗时：< 1 ms）
```

**实现：**
```c
void teacher_sortByRatingDesc(TeacherService* s) {
    seqlist_sort_by_rating_desc(s->teachers);
    printf("--- 教师按评分降序 ---\n");
    seqlist_traverse(s->teachers);
}
```

### 8.4 StudentService 业务功能

学生用 §8.2 的 `LinkedList` 存 `Student`。下面每条功能先给 I/O 再给实现，函数体在 `src/service/StudentService.c`。

#### 菜单

```
========== 学生管理 ==========
 1. 新增学生
 2. 删除学生（按学号）
 3. 修改学生
 4. 按学号查询
 5. 列出全部学生
 6. 按绩点排序
 7. 列出有不良记录的学生
 0. 返回主菜单
==============================
```

#### 【功能 1】新增学生

**输入：**
```
请输入选项: 1
--- 新增学生 ---
学号: S2023351
姓名: 测试学生
性别: 男
专业: 计算机
年级: 2023
绩点: 3.5
是否有不良记录(0/1): 0
```

**输出：**
```
✅ 学生添加成功！
```

**实现：**
```c
bool student_add(StudentService* s, Student stu) {
    if (!linkedlist_insert(s->students, stu)) return false;
    history_record("add", "Student", stu.id, "新增学生");
    printf("✅ 学生添加成功！\n");
    return true;
}
```

#### 【功能 2】删除学生

**输入：**
```
请输入选项: 2
请输入学号: S2023351
```

**输出：**
```
✅ 学生删除成功！
```

或：
```
❌ 未找到该学号
```

**实现：**
```c
bool student_remove(StudentService* s, const char* id) {
    if (!linkedlist_remove(s->students, id)) {
        printf("❌ 未找到该学号\n");
        return false;
    }
    history_record("remove", "Student", id, "删除学生");
    printf("✅ 学生删除成功！\n");
    return true;
}
```

#### 【功能 3】修改学生

**输入：**
```
请输入选项: 3
请输入学号: S2023010
新姓名: 赵一改
新性别(0女 1男): 1
新专业: 计算机
新年级: 2023
新绩点: 3.9
新不良级别: 0
```

**输出：**
```
✅ 学生修改成功！
```

**实现：**
```c
bool student_update(StudentService* s, const char* id, Student stu) {
    Student* p = linkedlist_find(s->students, id);
    if (!p) { printf("❌ 未找到该学号\n"); return false; }
    *p = stu;
    history_record("update", "Student", id, "修改学生");
    printf("✅ 学生修改成功！\n");
    return true;
}
```

#### 【功能 4】按学号查询

**输入：**
```
请输入选项: 4
请输入学号: S2023010
```

**输出：**
```
--- 查询结果 ---
学号：S2023010
姓名：赵一
性别：男
专业：计算机
年级：2023
绩点：3.8
不良记录：无
```

**实现：**
```c
Student* student_queryById(StudentService* s, const char* id) {
    Student* stu = linkedlist_find(s->students, id);
    if (!stu) { printf("❌ 未找到该学号\n"); return NULL; }
    printf("--- 查询结果 ---\n");
    printf("学号：%s\n姓名：%s\n性别：%s\n专业：%s\n年级：%d\n",
           stu->id, stu->name,
           stu->gender ? "男" : "女",
           stu->major, stu->grade);
    printf("绩点：%.1f\n不良记录：%s\n",
           stu->gpa,
           stu->hasBadRecord ? "有" : "无");
    return stu;
}
```

#### 【功能 5】列出全部学生

**输入：**
```
请输入选项: 5
```

**输出：**
```
--- 全部学生（共 350 人）---
[1]   学号 S2023001  姓名 ...  绩点 3.5
[2]   学号 S2023002  姓名 ...  绩点 3.8
[3]   学号 S2023003  姓名 ...  绩点 2.9
...
（每页 20 条，按回车继续）
```

**实现：**
```c
void student_listAll(StudentService* s) {
    printf("--- 全部学生（共 %d 人）---\n",
           linkedlist_size(s->students));
    linkedlist_traverse(s->students);   // LinkedList.c 中按 [N] 格式打印
}
```

#### 【功能 6】按绩点排序

**输入：**
```
请输入选项: 6
请输入 K（前 K 名）: 10
```

**输出：**
```
--- 学生按绩点降序（前 10 名）---
学号        姓名    绩点
S2023045    王五    4.9
S2023102    李四    4.8
S2023088    张三    4.7
S2023067    赵一    4.7
S2023123    钱二    4.6
...
（排序耗时：2.3 ms）
```

**实现：**
```c
void student_sortByGpaDesc(StudentService* s, int k) {
    // 复制一份链表再排序，避免破坏插入顺序
    LinkedList* copy = linkedlist_create();
    for (ListNode* p = s->students->head; p; p = p->next)
        linkedlist_insertTail(copy, p->data);
    linkedlist_sort_by_gpa_desc(copy);
    printf("--- 学生按绩点降序（前 %d 名）---\n", k);
    int shown = 0;
    for (ListNode* p = copy->head; p && shown < k; p = p->next, shown++)
        printf("学号\t%s\t姓名\t%s\t绩点\t%.1f\n",
               p->data.id, p->data.name, p->data.gpa);
    linkedlist_destroy(copy);
}
```

#### 【功能 7】列出有不良记录的学生

**输入：**
```
请输入选项: 7
```

**输出：**
```
--- 有不良记录的学生（共 35 人）---
学号        姓名    不良级别    绩点
S2023012    孙三    3           2.1
S2023034    钱二    2           2.5
S2023056    赵一    1           3.0
...
```

**实现：**
```c
void student_listBadRecords(StudentService* s) {
    int cnt = 0;
    for (ListNode* p = s->students->head; p; p = p->next)
        if (p->data.hasBadRecord) cnt++;
    printf("--- 有不良记录的学生（共 %d 人）---\n", cnt);
    for (ListNode* p = s->students->head; p; p = p->next)
        if (p->data.hasBadRecord)
            printf("学号\t%s\t姓名\t%s\t不良级别\t%d\t绩点\t%.1f\n",
                   p->data.id, p->data.name,
                   p->data.badLevel, p->data.gpa);
}
```

### 8.5 菜单实现

C 在自己的 service.c 里写菜单循环。TeacherService 和 StudentService 各一个。

#### 8.5.1 teacher_service_menu（在 TeacherService.c）

```c
void teacher_service_menu(TeacherService* s) {
    int choice;
    while (true) {
        printf("\n========== 教师管理 ==========\n");
        printf(" 1. 新增教师\n");
        printf(" 2. 删除教师（按工号）\n");
        printf(" 3. 修改教师\n");
        printf(" 4. 按工号查询\n");
        printf(" 5. 列出全部教师\n");
        printf(" 6. 按授课评分排序\n");
        printf(" 0. 返回主菜单\n");
        printf("==============================\n");
        printf("请输入选项: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: {
                Teacher t; memset(&t, 0, sizeof(t));
                printf("工号: ");   scanf("%s", t.id);
                printf("姓名: ");   scanf("%s", t.name);
                printf("职称: ");   scanf("%s", t.title);
                printf("院系: ");   scanf("%s", t.department);
                printf("评分: ");   scanf("%lf", &t.rating);
                teacher_add(s, t)
                    ? printf("✅ 教师添加成功！\n")
                    : printf("❌ 教师添加失败\n");
                break;
            }
            case 2: {
                char id[MAX_ID_LEN];
                printf("请输入工号: "); scanf("%s", id);
                teacher_remove(s, id);
                break;
            }
            case 3: {
                char id[MAX_ID_LEN]; Teacher t; memset(&t, 0, sizeof(t));
                printf("请输入工号: "); scanf("%s", id);
                printf("新姓名: ");   scanf("%s", t.name);
                printf("新职称: ");   scanf("%s", t.title);
                printf("新院系: ");   scanf("%s", t.department);
                printf("新评分: ");   scanf("%lf", &t.rating);
                strncpy(t.id, id, MAX_ID_LEN);
                teacher_update(s, id, t);
                break;
            }
            case 4: {
                char id[MAX_ID_LEN];
                printf("请输入工号: "); scanf("%s", id);
                teacher_queryById(s, id);
                break;
            }
            case 5: teacher_listAll(s);         break;
            case 6: teacher_sortByRatingDesc(s); break;
            case 0: return;
            default: printf("❌ 无效选项\n");
        }
    }
}
```

#### 8.5.2 student_service_menu（在 StudentService.c）

```c
void student_service_menu(StudentService* s) {
    int choice;
    while (true) {
        printf("\n========== 学生管理 ==========\n");
        printf(" 1. 新增学生\n");
        printf(" 2. 删除学生（按学号）\n");
        printf(" 3. 修改学生\n");
        printf(" 4. 按学号查询\n");
        printf(" 5. 列出全部学生\n");
        printf(" 6. 按绩点排序\n");
        printf(" 7. 列出有不良记录的学生\n");
        printf(" 0. 返回主菜单\n");
        printf("==============================\n");
        printf("请输入选项: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: {
                Student stu; memset(&stu, 0, sizeof(stu));
                printf("学号: "); scanf("%s", stu.id);
                printf("姓名: "); scanf("%s", stu.name);
                int g;        printf("性别(0女 1男): "); scanf("%d", &g);
                stu.gender = g;
                printf("专业: "); scanf("%s", stu.major);
                printf("年级: "); scanf("%d", &stu.grade);
                printf("绩点: "); scanf("%lf", &stu.gpa);
                printf("是否有不良记录(0/1): "); scanf("%d", &stu.hasBadRecord);
                if (stu.hasBadRecord) {
                    printf("不良级别(1-3): "); scanf("%d", &stu.badLevel);
                }
                student_add(s, stu);
                break;
            }
            case 2: {
                char id[MAX_ID_LEN];
                printf("请输入学号: "); scanf("%s", id);
                student_remove(s, id);
                break;
            }
            case 3: {
                char id[MAX_ID_LEN]; Student stu; memset(&stu, 0, sizeof(stu));
                printf("请输入学号: "); scanf("%s", id);
                printf("新姓名: "); scanf("%s", stu.name);
                int g; printf("新性别(0女 1男): "); scanf("%d", &g);
                stu.gender = g;
                printf("新专业: "); scanf("%s", stu.major);
                printf("新年级: "); scanf("%d", &stu.grade);
                printf("新绩点: "); scanf("%lf", &stu.gpa);
                printf("新不良级别(0=无): "); scanf("%d", &stu.badLevel);
                stu.hasBadRecord = (stu.badLevel > 0);
                strncpy(stu.id, id, MAX_ID_LEN);
                student_update(s, id, stu);
                break;
            }
            case 4: {
                char id[MAX_ID_LEN];
                printf("请输入学号: "); scanf("%s", id);
                student_queryById(s, id);
                break;
            }
            case 5: student_listAll(s);        break;
            case 6: {
                int k; printf("请输入 K（前 K 名）: "); scanf("%d", &k);
                student_sortByGpaDesc(s, k);
                break;
            }
            case 7: student_listBadRecords(s); break;
            case 0: return;
            default: printf("❌ 无效选项\n");
        }
    }
}
```

### 8.6 TeacherService + StudentService 接口定义

```c
// ----- TeacherService -----
typedef struct {
    SeqList* teachers;
} TeacherService;

TeacherService* teacher_service_create(void);
void            teacher_service_destroy(TeacherService* s);

bool     teacher_add(TeacherService* s, Teacher t);
bool     teacher_remove(TeacherService* s, const char* id);
bool     teacher_update(TeacherService* s, const char* id, Teacher t);
Teacher* teacher_queryById(TeacherService* s, const char* id);
void     teacher_listAll(TeacherService* s);
void     teacher_sortByRatingDesc(TeacherService* s);

void     teacher_service_menu(TeacherService* s);   // 见 §8.5.1

// ----- StudentService -----
typedef struct {
    LinkedList* students;
} StudentService;

StudentService* student_service_create(void);
void            student_service_destroy(StudentService* s);

bool     student_add(StudentService* s, Student stu);
bool     student_remove(StudentService* s, const char* id);
bool     student_update(StudentService* s, const char* id, Student stu);
Student* student_queryById(StudentService* s, const char* id);
void     student_listAll(StudentService* s);
void     student_sortByGpaDesc(StudentService* s, int k);
void     student_listBadRecords(StudentService* s);

void     student_service_menu(StudentService* s);   // 见 §8.5.2
```

---

## 九、组员 D —— BST + 哈希表 + SearchService

### 9.1 负责的文件

| 文件 | 说明 |
|------|------|
| `src/ds/BST.h/.c` | 二叉排序树（key=GPA，存 Student）|
| `src/ds/HashTable.h/.c` | 哈希表（key=学号/工号，value=Student*）|
| `src/service/SearchService.h/.c` | 查找服务 |

### 9.2 BST + HashTable + TeacherHashTable 写法

三种数据结构都直接存具体业务类型。`TeacherHashTable` 是与 `HashTable`（存 Student）**并列**的另一个类型（与 §6.5 的 `CourseList` 同模式），**不用 `void*`**，保证类型安全。

```c
// ----- BST.h（key=double，存 Student）-----
typedef struct BSTNode {
    double key;
    Student value;
    struct BSTNode* left;
    struct BSTNode* right;
} BSTNode;

typedef struct {
    BSTNode* root;
    int size;
} BST;

// 创建空 BST
BST* bst_create(void);
// 释放 BST 内存（含所有节点）
void bst_destroy(BST* tree);

// 插入 (key, value)；key 已存在则覆盖 value 并返回 true
bool    bst_insert(BST* tree, double key, Student value);
// 按 key 删除节点；未找到返回 false
bool    bst_remove(BST* tree, double key);
// 按 key 查找；返回指向节点内 Student 的指针（不要 free），未找到返回 NULL
Student* bst_find(BST* tree, double key);

// 范围查询 [lowKey, highKey]，结果按 key 升序写入 result；result 需预分配足够空间
void bst_rangeQuery(BST* tree, double lowKey, double highKey,
                    Student* result, int* count);
// 中序遍历全部节点，结果按 key 升序写入 result；result 需预分配至少 size 个
void bst_inorder(BST* tree, Student* result, int* count);

// 当前 BST 中节点个数
int  bst_size(BST* tree);
// 清空整棵树（root = NULL，size = 0）
void bst_clear(BST* tree);

// ----- HashTable.h（key=char[]，value=Student*）-----
typedef struct HashNode {
    char key[MAX_ID_LEN];
    Student* value;
    struct HashNode* next;
} HashNode;

typedef struct {
    HashNode** buckets;
    int size;
    int capacity;
} HashTable;

// 创建学生哈希表（capacity 指定桶数，建议略大于预计元素数）
HashTable* hashtable_create(int capacity);
// 释放哈希表内存（含所有桶链表节点）
void       hashtable_destroy(HashTable* ht);

// 插入 (key, s)；key 已存在则覆盖并返回 true
bool     hashtable_insert(HashTable* ht, const char* key, Student* s);
// 按 key 删除节点；未找到返回 false
bool     hashtable_remove(HashTable* ht, const char* key);
// 按 key 查找；返回 Student*（哈希表只存指针，不复制 Student），未找到返回 NULL
Student* hashtable_find(HashTable* ht, const char* key);
// 仅判断 key 是否存在（不返回值）
bool     hashtable_contains(HashTable* ht, const char* key);
// 当前哈希表中元素个数
int      hashtable_size(HashTable* ht);

// 所有桶链中最长链的长度（§9.3 功能 6 哈希状态用）
int    hashtable_getLongestChain(HashTable* ht);
// 装填因子 = size / capacity（§9.3 功能 5/6 用）
double hashtable_getLoadFactor(HashTable* ht);
// 打印表格容量/已存/装填因子/最长链/桶分布图（§9.3 功能 6 用）
void   hashtable_printStats(HashTable* ht);

// ----- TeacherHashTable.h（key=char[]，value=Teacher*，与 HashTable 并列）-----
typedef struct TeacherHashNode {
    char key[MAX_ID_LEN];
    Teacher* value;
    struct TeacherHashNode* next;
} TeacherHashNode;

typedef struct {
    TeacherHashNode** buckets;
    int size;
    int capacity;
} TeacherHashTable;

// 创建教师哈希表（与学生哈希表 API 一致，类型不同；§9.2 并列模式避免 void*）
TeacherHashTable* teacher_hashtable_create(int capacity);
// 释放教师哈希表内存（含所有桶链表节点）
void              teacher_hashtable_destroy(TeacherHashTable* ht);

// 插入 (key, t)；key 已存在则覆盖并返回 true
bool    teacher_hashtable_insert(TeacherHashTable* ht, const char* key, Teacher* t);
// 按 key 删除节点；未找到返回 false
bool    teacher_hashtable_remove(TeacherHashTable* ht, const char* key);
// 按 key 查找；返回 Teacher*（哈希表只存指针），未找到返回 NULL
Teacher* teacher_hashtable_find(TeacherHashTable* ht, const char* key);
// 仅判断 key 是否存在
bool    teacher_hashtable_contains(TeacherHashTable* ht, const char* key);
// 当前教师哈希表中元素个数
int     teacher_hashtable_size(TeacherHashTable* ht);
```

### 9.3 SearchService 业务功能

查找服务组合 §9.2 的三种结构。下面每条功能先给 I/O 再给实现，函数体在 `src/service/SearchService.c`。

#### 菜单

```
========== 查找服务 ==========
 1. 按学号精确查找学生（哈希表）
 2. 按工号精确查找教师（哈希表）
 3. 按绩点区间查找学生（BST）
 4. 按不良记录严重程度区间查找（BST）
 5. 性能对比演示（哈希 vs BST）
 6. 哈希表状态查看
 0. 返回主菜单
==============================
```

#### 【功能 1】按学号精确查找学生（哈希表）

**输入：**
```
请输入选项: 1
请输入学号: S2023010
```

**输出：**
```
[哈希计算] key="S2023010" → bucket=42
--- 查询结果 ---
学号：S2023010
姓名：赵一
性别：男
专业：计算机
年级：2023
绩点：3.8
不良记录：无
⏱️ 查找耗时：0.42 μs
```

或：
```
❌ 未找到该学号
```

**实现：**
```c
Student* search_findStudentById(SearchService* s, const char* id) {
    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);
    int bucket = hash_str(id) % s->studentsById->capacity;
    printf("[哈希计算] key=\"%s\" → bucket=%d\n", id, bucket);
    Student* stu = hashtable_find(s->studentsById, id);
    clock_gettime(CLOCK_MONOTONIC, &t1);
    if (!stu) { printf("❌ 未找到该学号\n"); return NULL; }
    printf("--- 查询结果 ---\n");
    printf("学号：%s\n姓名：%s\n性别：%s\n专业：%s\n年级：%d\n绩点：%.1f\n不良记录：%s\n",
           stu->id, stu->name, stu->gender, stu->major, stu->grade, stu->gpa,
           stu->hasBadRecord ? "有" : "无");
    double us = (t1.tv_sec - t0.tv_sec) * 1e6 + (t1.tv_nsec - t0.tv_nsec) / 1e3;
    printf("⏱️ 查找耗时：%.2f μs\n", us);
    return stu;
}
```

#### 【功能 2】按工号精确查找教师（哈希表）

**输入：**
```
请输入选项: 2
请输入工号: T102
```

**输出：**
```
[哈希计算] key="T102" → bucket=89
--- 查询结果 ---
工号：T102
姓名：王芳
职称：副教授
院系：计算机学院
授课评分：4.3
⏱️ 查找耗时：0.38 μs
```

或：
```
❌ 未找到该工号
```

**实现：**
```c
Teacher* search_findTeacherById(SearchService* s, const char* id) {
    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);
    int bucket = hash_str(id) % s->teachersById->capacity;
    printf("[哈希计算] key=\"%s\" → bucket=%d\n", id, bucket);
    Teacher* t = teacher_hashtable_find(s->teachersById, id);
    clock_gettime(CLOCK_MONOTONIC, &t1);
    if (!t) { printf("❌ 未找到该工号\n"); return NULL; }
    printf("--- 查询结果 ---\n");
    printf("工号：%s\n姓名：%s\n职称：%s\n院系：%s\n授课评分：%.1f\n",
           t->id, t->name, t->title, t->department, t->rating);
    double us = (t1.tv_sec - t0.tv_sec) * 1e6 + (t1.tv_nsec - t0.tv_nsec) / 1e3;
    printf("⏱️ 查找耗时：%.2f μs\n", us);
    return t;
}
```

#### 【功能 3】按绩点区间查找学生（BST）

**输入：**
```
请输入选项: 3
请输入绩点下限: 3.5
请输入绩点上限: 4.0
```

**输出：**
```
[BST 中序遍历] 范围 [3.5, 4.0]
--- 绩点在 [3.5, 4.0] 的学生（共 87 人）---
学号        姓名    绩点
S2023010    赵一    3.5
S2023023    钱二    3.5
S2023056    孙三    3.6
...
S2023499    王五    4.0
⏱️ 查找耗时：12.3 μs
```

**实现：**
```c
void search_findStudentsByGPA(SearchService* s, double low, double high) {
    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);
    printf("[BST 中序遍历] 范围 [%.1f, %.1f]\n", low, high);
    Student buf[500];
    int cnt = 0;
    bst_rangeQuery(s->studentsByGpa, low, high, buf, &cnt);
    printf("--- 绩点在 [%.1f, %.1f] 的学生（共 %d 人）---\n", low, high, cnt);
    printf("学号\t姓名\t绩点\n");
    for (int i = 0; i < cnt; i++)
        printf("%s\t%s\t%.1f\n", buf[i].id, buf[i].name, buf[i].gpa);
    clock_gettime(CLOCK_MONOTONIC, &t1);
    double us = (t1.tv_sec - t0.tv_sec) * 1e6 + (t1.tv_nsec - t0.tv_nsec) / 1e3;
    printf("⏱️ 查找耗时：%.2f μs\n", us);
}
```

#### 【功能 4】按不良记录严重程度区间查找（BST）

**输入：**
```
请输入选项: 4
请输入不良级别下限: 2
请输入不良级别上限: 3
```

**输出：**
```
[BST 中序遍历] 范围 [2, 3]
--- 不良级别在 [2, 3] 的学生（共 23 人）---
学号        姓名    不良级别    绩点
S2023012    孙三    3           2.1
S2023034    钱二    2           2.5
S2023078    王五    3           2.3
...
```

**实现：**
```c
void search_findStudentsByBadLevel(SearchService* s, int low, int high) {
    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);
    printf("[BST 中序遍历] 范围 [%d, %d]\n", low, high);
    Student buf[200];
    int cnt = 0;
    bst_rangeQuery(s->studentsByBadLevel, low, high, buf, &cnt);
    printf("--- 不良级别在 [%d, %d] 的学生（共 %d 人）---\n", low, high, cnt);
    for (int i = 0; i < cnt; i++)
        printf("%s\t%s\t%d\t%.1f\n",
               buf[i].id, buf[i].name, buf[i].badLevel, buf[i].gpa);
    clock_gettime(CLOCK_MONOTONIC, &t1);
    double us = (t1.tv_sec - t0.tv_sec) * 1e6 + (t1.tv_nsec - t0.tv_nsec) / 1e3;
    printf("⏱️ 查找耗时：%.2f μs\n", us);
}
```

#### 【功能 5】性能对比演示（验收亮点）

**输入：**
```
请输入选项: 5
请输入测试次数: 1000
```

**输出：**
```
[生成 1000 个随机学号...]
[开始测试哈希表...]
 哈希表查找 1000 次：423.5 μs
  平均每次：0.42 μs

[开始测试 BST（中序遍历代替精确查找）...]
  BST 遍历 1000 次：12580.2 μs
  平均每次：12.58 μs

📊 性能对比：
  哈希表查找比 BST 快约 29.7 倍
  数据规模：350 学生，hash 桶数 100，装填因子 3.50
```

**实现：** BST 没有按 ID 查找的接口，所以公平对比是「哈希 O(1) vs BST 中序遍历 O(n)」。

```c
void search_benchmark(SearchService* s, int testCount) {
    char ids[1000][MAX_ID_LEN];
    for (int i = 0; i < testCount; i++)
        snprintf(ids[i], MAX_ID_LEN, "S%07d", 2023001 + rand() % 350);

    struct timespec t0, t1;

    // 哈希表测试
    clock_gettime(CLOCK_MONOTONIC, &t0);
    for (int i = 0; i < testCount; i++)
        hashtable_find(s->studentsById, ids[i]);
    clock_gettime(CLOCK_MONOTONIC, &t1);
    double hash_us = (t1.tv_sec - t0.tv_sec) * 1e6
                   + (t1.tv_nsec - t0.tv_nsec) / 1e3;

    // BST 测试（中序遍历出全部 student，再线性扫描找 ID）
    clock_gettime(CLOCK_MONOTONIC, &t0);
    Student all[400]; int n = 0;
    bst_inorder(s->studentsByGpa, all, &n);
    for (int i = 0; i < testCount; i++)
        for (int j = 0; j < n; j++)
            if (strcmp(all[j].id, ids[i]) == 0) break;
    clock_gettime(CLOCK_MONOTONIC, &t1);
    double bst_us = (t1.tv_sec - t0.tv_sec) * 1e6
                  + (t1.tv_nsec - t0.tv_nsec) / 1e3;

    printf("📊 性能对比：\n");
    printf("  哈希表查找比 BST 快约 %.1f 倍\n", bst_us / hash_us);
    printf("  数据规模：350 学生，hash 桶数 %d，装填因子 %.2f\n",
           s->studentsById->capacity,
           hashtable_getLoadFactor(s->studentsById));
}
```

#### 【功能 6】哈希表状态查看

**输入：**
```
请输入选项: 6
```

**输出：**
```
--- 学生哈希表状态 ---
容量：100 桶
已存：350 项
装填因子：3.50
最长链长度：8
平均链长度：3.50
[哈希表桶分布图]
桶  0~9:   ### ##  #   #  ####
桶 10~19:  ## #### #   ##  ##
桶 20~29:  ####  #  ##  ### ##
...
```

**实现：**
```c
void search_printHashStats(SearchService* s) {
    printf("--- 学生哈希表状态 ---\n");
    hashtable_printStats(s->studentsById);
}
```

### 9.4 菜单实现

D 在 SearchService.c 末尾写 `search_service_menu`，6 个功能对应 6 个 case。

```c
void search_service_menu(SearchService* s) {
    int choice;
    while (true) {
        printf("\n========== 查找服务 ==========\n");
        printf(" 1. 按学号精确查找学生（哈希表）\n");
        printf(" 2. 按工号精确查找教师（哈希表）\n");
        printf(" 3. 按绩点区间查找学生（BST）\n");
        printf(" 4. 按不良记录严重程度区间查找（BST）\n");
        printf(" 5. 性能对比演示（哈希 vs BST）\n");
        printf(" 6. 哈希表状态查看\n");
        printf(" 0. 返回主菜单\n");
        printf("==============================\n");
        printf("请输入选项: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: {
                char id[MAX_ID_LEN];
                printf("请输入学号: "); scanf("%s", id);
                search_findStudentById(s, id);
                break;
            }
            case 2: {
                char id[MAX_ID_LEN];
                printf("请输入工号: "); scanf("%s", id);
                search_findTeacherById(s, id);
                break;
            }
            case 3: {
                double lo, hi;
                printf("请输入绩点下限: "); scanf("%lf", &lo);
                printf("请输入绩点上限: "); scanf("%lf", &hi);
                search_findStudentsByGPA(s, lo, hi);
                break;
            }
            case 4: {
                int lo, hi;
                printf("请输入不良级别下限: "); scanf("%d", &lo);
                printf("请输入不良级别上限: "); scanf("%d", &hi);
                search_findStudentsByBadLevel(s, lo, hi);
                break;
            }
            case 5: {
                int n; printf("请输入测试次数: "); scanf("%d", &n);
                search_benchmark(s, n);
                break;
            }
            case 6: search_printHashStats(s); break;
            case 0: return;
            default: printf("❌ 无效选项\n");
        }
    }
}
```

### 9.5 SearchService 接口定义

```c
typedef struct {
    HashTable*        studentsById;
    TeacherHashTable* teachersById;    // 与 §9.2 TeacherHashTable 对应
    BST*              studentsByGpa;
    BST*              studentsByBadLevel;

    StudentService* studentSvc;
    TeacherService* teacherSvc;
} SearchService;

SearchService* search_service_create(StudentService* ss, TeacherService* ts);
void            search_service_destroy(SearchService* s);

void search_service_rebuildIndexes(SearchService* s);

Student* search_findStudentById(SearchService* s, const char* id);
Teacher* search_findTeacherById(SearchService* s, const char* id);
void    search_findStudentsByGPA(SearchService* s, double low, double high);
void    search_findStudentsByBadLevel(SearchService* s, int low, int high);
void    search_benchmark(SearchService* s, int testCount);
void    search_printHashStats(SearchService* s);

void    search_service_menu(SearchService* s);   // 见 §9.4
```

---

## 十、组员 E —— 堆 + ScoreService + RankingService

### 10.1 负责的文件

| 文件 | 说明 |
|------|------|
| `src/ds/Heap.h/.c` | 堆（存 Student）|
| `src/service/ScoreService.h/.c` | 成绩管理 |
| `src/service/RankingService.h/.c` | TopK 排行榜 |

### 10.2 Heap.h 写法（存 Student）

```c
typedef struct {
    Student* data;
    int size;
    int capacity;
    bool isMinHeap;     // true=小顶堆（升序），false=大顶堆（降序）
} Heap;

// 创建堆（isMinHeap=true → 小顶堆，返回最小值；false → 大顶堆，返回最大值）
Heap* heap_create(bool isMinHeap);
// 释放堆内存（含内部 Student 数组）
void  heap_destroy(Heap* h);

// 向堆中插入一个 Student，自动 sift up 维持堆序；返回 false 表示堆已满
bool    heap_insert(Heap* h, Student s);
// 取出堆顶元素（最小或最大），自动 sift down 维持堆序；堆空时返回全零 Student
Student heap_extractTop(Heap* h);
// 查看堆顶元素但不出队；堆空时返回全零 Student
Student heap_peek(Heap* h);

// 当前堆中元素个数
int     heap_size(Heap* h);
// 堆是否为空（size == 0）
bool    heap_isEmpty(Heap* h);

// 对 Student 数组做堆排序；isMinHeap=true 升序，false 降序；原地排序，arr 内容会被改写
void heap_sort(Student* arr, int n, bool isMinHeap);
// 从 all[0..n) 中取前 k 大的元素，写入 result[0..k)；result 需预先分配好 k 大小
void heap_topK_max(Student* all, int n, int k, Student* result);
```

### 10.3 ScoreService 业务功能

成绩用 `ScoreList`（与 §6.5 `CourseList` 同模式，存 `Score` 而非泛型）。下面每条功能先给 I/O 再给实现，函数体在 `src/service/ScoreService.c`。

#### 菜单

```
========== 成绩管理 ==========
 1. 新增成绩记录
 2. 删除成绩记录
 3. 按学号查询某学生所有成绩
 4. 按课程号查询某课程所有成绩
 5. 按分数排序（堆排序）
 6. 计算某学生平均分
 7. 计算某课程平均分
 0. 返回主菜单
==============================
```

#### 【功能 1】新增成绩记录

**输入：**
```
请输入选项: 1
--- 新增成绩 ---
学号: S2023010
课程号: C006
分数: 92.5
考试类型(期中/期末/平时): 期末
```

**输出：**
```
✅ 成绩记录已添加
```

**实现：**
```c
bool score_add(ScoreService* s, Score sc) {
    if (!scorelist_insert(s->scores, sc)) return false;
    history_record("add", "Score", sc.studentId, "新增成绩");
    printf("✅ 成绩记录已添加\n");
    return true;
}
```

#### 【功能 2】删除成绩记录

**输入：**
```
请输入选项: 2
请输入学号: S2023010
请输入课程号: C001
```

**输出：**
```
✅ 成绩记录已删除
```

或：
```
❌ 未找到该成绩记录
```

**实现：**
```c
bool score_remove(ScoreService* s, const char* studentId,
                  const char* courseId) {
    if (!scorelist_remove(s->scores, studentId, courseId)) {
        printf("❌ 未找到该成绩记录\n");
        return false;
    }
    history_record("remove", "Score", studentId, "删除成绩");
    printf("✅ 成绩记录已删除\n");
    return true;
}
```

#### 【功能 3】按学号查询某学生所有成绩

**输入：**
```
请输入选项: 3
请输入学号: S2023010
```

**输出：**
```
--- S2023010 赵一 的成绩 ---
课程号   课程名       分数    类型
C001     数据结构     85.0    期末
C002     算法分析     92.0    期末
C003     数据库原理   78.0    期中
平均分：85.0
```

**实现：**
```c
void score_findByStudent(ScoreService* s, const char* studentId) {
    Student* stu = student_queryById(s->studentSvc, studentId);
    if (!stu) { printf("❌ 未找到该学生\n"); return; }
    printf("--- %s %s 的成绩 ---\n", stu->id, stu->name);
    double sum = 0; int cnt = 0;
    for (int i = 0; i < scorelist_size(s->scores); i++) {
        Score* sc = scorelist_at(s->scores, i);
        if (strcmp(sc->studentId, studentId) != 0) continue;
        Course* c = course_queryById(s->courseSvc, sc->courseId);
        printf("%s\t%s\t%.1f\t%s\n",
               sc->courseId, c ? c->name : "?",
               sc->score, sc->type);
        sum += sc->score; cnt++;
    }
    if (cnt > 0) printf("平均分：%.1f\n", sum / cnt);
}
```

#### 【功能 4】按课程号查询某课程所有成绩

**输入：**
```
请输入选项: 4
请输入课程号: C001
```

**输出：**
```
--- C001 数据结构 成绩（共 78 条）---
学号        姓名    分数
S2023001    张三    88.0
S2023002    李四    92.0
...
平均分：82.5
```

**实现：**
```c
void score_findByCourse(ScoreService* s, const char* courseId) {
    Course* c = course_queryById(s->courseSvc, courseId);
    if (!c) { printf("❌ 未找到该课程\n"); return; }
    double sum = 0; int cnt = 0;
    for (int i = 0; i < scorelist_size(s->scores); i++) {
        Score* sc = scorelist_at(s->scores, i);
        if (strcmp(sc->courseId, courseId) != 0) continue;
        Student* stu = student_queryById(s->studentSvc, sc->studentId);
        printf("%s\t%s\t%.1f\n",
               sc->studentId, stu ? stu->name : "?", sc->score);
        sum += sc->score; cnt++;
    }
    printf("--- %s %s 成绩（共 %d 条）---\n", c->id, c->name, cnt);
    if (cnt > 0) printf("平均分：%.1f\n", sum / cnt);
}
```

#### 【功能 5】按分数排序（堆排序）

**输入：**
```
请输入选项: 5
请输入显示条数: 10
```

**输出：**
```
[堆排序] 数据规模 1400 条
--- 全部成绩按分数降序（前 10 条）---
学号        课程号   分数
S2023045    C002     98.0
S2023088    C001     96.5
S2023012    C003     95.0
S2023078    C002     94.0
...
⏱️ 堆排序耗时：234 μs
```

**实现：** 拷贝所有 Score 进堆数组，堆排序后取前 K 条。

```c
void score_sortByScoreDesc(ScoreService* s, int k) {
    int n = scorelist_size(s->scores);
    Score* arr = malloc(sizeof(Score) * n);
    for (int i = 0; i < n; i++) *arr++ = *scorelist_at(s->scores, i);
    arr -= n;

    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);
    heap_sort(arr, n, false /* 大顶堆 */);   // Heap.c 中实现
    clock_gettime(CLOCK_MONOTONIC, &t1);

    printf("[堆排序] 数据规模 %d 条\n", n);
    printf("--- 全部成绩按分数降序（前 %d 条）---\n", k);
    int shown = k < n ? k : n;
    for (int i = 0; i < shown; i++)
        printf("%s\t%s\t%.1f\n",
               arr[i].studentId, arr[i].courseId, arr[i].score);
    free(arr);
    double us = (t1.tv_sec - t0.tv_sec) * 1e6 + (t1.tv_nsec - t0.tv_nsec) / 1e3;
    printf("⏱️ 堆排序耗时：%.0f μs\n", us);
}
```

#### 【功能 6】计算某学生平均分

**输入：**
```
请输入选项: 6
请输入学号: S2023010
```

**输出：**
```
--- S2023010 赵一 成绩统计 ---
课程数：3
总分：255.0
平均分：85.0
对应平均绩点：3.6
```

**实现：**
```c
void score_avgByStudent(ScoreService* s, const char* studentId) {
    Student* stu = student_queryById(s->studentSvc, studentId);
    if (!stu) { printf("❌ 未找到该学生\n"); return; }
    double sum = 0; int cnt = 0;
    for (int i = 0; i < scorelist_size(s->scores); i++) {
        Score* sc = scorelist_at(s->scores, i);
        if (strcmp(sc->studentId, studentId) == 0) { sum += sc->score; cnt++; }
    }
    double avg = cnt ? sum / cnt : 0;
    printf("--- %s %s 成绩统计 ---\n", stu->id, stu->name);
    printf("课程数：%d\n总分：%.1f\n平均分：%.1f\n", cnt, sum, avg);
    printf("对应平均绩点：%.1f\n", local_score_to_gpa(avg));   // 私有换算，见下
}

// 分数→绩点（4.0 制）：本模块私有，**不**对外暴露，避免 §6.3 跨服务依赖
static double local_score_to_gpa(double score) {
    if (score >= 90) return 4.0;
    if (score >= 85) return 3.7;
    if (score >= 82) return 3.3;
    if (score >= 78) return 3.0;
    if (score >= 75) return 2.7;
    if (score >= 72) return 2.3;
    if (score >= 68) return 2.0;
    if (score >= 64) return 1.5;
    if (score >= 60) return 1.0;
    return 0;
}
```

#### 【功能 7】计算某课程平均分

**输入：**
```
请输入选项: 7
请输入课程号: C001
```

**输出：**
```
--- C001 数据结构 成绩统计 ---
学生数：78
总分：6435.0
平均分：82.5
```

**实现：**
```c
void score_avgByCourse(ScoreService* s, const char* courseId) {
    Course* c = course_queryById(s->courseSvc, courseId);
    if (!c) { printf("❌ 未找到该课程\n"); return; }
    double sum = 0; int cnt = 0;
    for (int i = 0; i < scorelist_size(s->scores); i++) {
        Score* sc = scorelist_at(s->scores, i);
        if (strcmp(sc->courseId, courseId) == 0) { sum += sc->score; cnt++; }
    }
    double avg = cnt ? sum / cnt : 0;
    printf("--- %s %s 成绩统计 ---\n", c->id, c->name);
    printf("学生数：%d\n总分：%.1f\n平均分：%.1f\n", cnt, sum, avg);
}
```

### 10.4 RankingService 业务功能

排行榜用 §10.2 的 `Heap` 做 TopK。下面每条功能先给 I/O 再给实现，函数体在 `src/service/RankingService.c`。

#### 菜单

```
========== 排行榜 ==========
 1. 学生绩点 TopK
 2. 教师授课评分 TopK
 0. 返回主菜单
==============================
```

#### 【功能 1】学生绩点 TopK（堆 TopK 算法）

**输入：**
```
请输入选项: 1
请输入 K 值: 10
```

**输出：**
```
[堆 TopK 算法] 数据规模 350，提取 Top 10
--- 学生绩点 Top 10 ---
排名  学号        姓名     绩点
 1    S2023045    王五     4.9
 2    S2023102    李四     4.8
 3    S2023088    张三     4.7
 4    S2023067    赵一     4.7
 5    S2023123    钱二     4.6
 6    S2023099    孙三     4.6
 7    S2023156    周八     4.5
 8    S2023078    吴九     4.5
 9    S2023145    郑十     4.4
10    S2023112    王芳     4.4
⏱️ TopK 耗时：89 μs
```

**实现：** 大小为 K 的小顶堆遍历全部 Student：若新元素 > 堆顶则替换堆顶，O(n log k)。

```c
void ranking_topKStudents(StudentService* ss, int k) {
    int n = linkedlist_size(ss->students);
    Student* all = malloc(sizeof(Student) * n);
    int idx = 0;
    for (ListNode* p = ss->students->head; p; p = p->next)
        all[idx++] = p->data;

    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);
    Student* top = malloc(sizeof(Student) * k);
    heap_topK_max(all, n, k, top);   // Heap.c 实现，返回前 K 大
    clock_gettime(CLOCK_MONOTONIC, &t1);

    printf("[堆 TopK 算法] 数据规模 %d，提取 Top %d\n", n, k);
    printf("--- 学生绩点 Top %d ---\n", k);
    printf("排名\t学号\t姓名\t绩点\n");
    for (int i = 0; i < k; i++)
        printf("%d\t%s\t%s\t%.1f\n", i + 1,
               top[i].id, top[i].name, top[i].gpa);

    free(all); free(top);
    double us = (t1.tv_sec - t0.tv_sec) * 1e6 + (t1.tv_nsec - t0.tv_nsec) / 1e3;
    printf("⏱️ TopK 耗时：%.0f μs\n", us);
}
```

#### 【功能 2】教师授课评分 TopK

**输入：**
```
请输入选项: 2
请输入 K 值: 5
```

**输出：**
```
[堆 TopK 算法] 数据规模 15，提取 Top 5
--- 教师授课评分 Top 5 ---
排名  工号   姓名   评分
 1    T105   杨帆   4.9
 2    T101   张伟   4.8
 3    T108   吴敏   4.7
 4    T104   陈静   4.5
 5    T110   孙丽   4.4
⏱️ TopK 耗时：12 μs
```

**实现：**
```c
void ranking_topKTeachers(TeacherService* ts, int k) {
    int n = seqlist_size(ts->teachers);
    Teacher* top = malloc(sizeof(Teacher) * k);
    heap_topK_max((Student*)seqlist_at(ts->teachers, 0), n, k, (Student*)top);
    // ↑ 这里复用 heap_topK_max 因为 Teacher 字段布局与 Student 兼容
    // 若不兼容，则单独写 teacher_topK 函数

    printf("[堆 TopK 算法] 数据规模 %d，提取 Top %d\n", n, k);
    printf("--- 教师授课评分 Top %d ---\n", k);
    for (int i = 0; i < k; i++)
        printf("%d\t%s\t%s\t%.1f\n", i + 1,
               top[i].id, top[i].name, top[i].rating);
    free(top);
}
```

### 10.5 菜单实现

E 在自己的 service.c 里写菜单循环。ScoreService 7 个功能 + RankingService 2 个功能 = 两个菜单函数。

#### 10.5.1 score_service_menu（在 ScoreService.c）

```c
void score_service_menu(ScoreService* s) {
    int choice;
    while (true) {
        printf("\n========== 成绩管理 ==========\n");
        printf(" 1. 新增成绩记录\n");
        printf(" 2. 删除成绩记录\n");
        printf(" 3. 按学号查询某学生所有成绩\n");
        printf(" 4. 按课程号查询某课程所有成绩\n");
        printf(" 5. 按分数排序（堆排序）\n");
        printf(" 6. 计算某学生平均分\n");
        printf(" 7. 计算某课程平均分\n");
        printf(" 0. 返回主菜单\n");
        printf("==============================\n");
        printf("请输入选项: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: {
                Score sc; memset(&sc, 0, sizeof(sc));
                printf("学号: ");   scanf("%s", sc.studentId);
                printf("课程号: "); scanf("%s", sc.courseId);
                printf("分数: ");   scanf("%lf", &sc.score);
                printf("考试类型(期中/期末/平时): "); scanf("%s", sc.type);
                score_add(s, sc);
                break;
            }
            case 2: {
                char sid[MAX_ID_LEN], cid[MAX_ID_LEN];
                printf("请输入学号: ");   scanf("%s", sid);
                printf("请输入课程号: "); scanf("%s", cid);
                score_remove(s, sid, cid);
                break;
            }
            case 3: {
                char sid[MAX_ID_LEN];
                printf("请输入学号: "); scanf("%s", sid);
                score_findByStudent(s, sid);
                break;
            }
            case 4: {
                char cid[MAX_ID_LEN];
                printf("请输入课程号: "); scanf("%s", cid);
                score_findByCourse(s, cid);
                break;
            }
            case 5: {
                int k; printf("请输入显示条数: "); scanf("%d", &k);
                score_sortByScoreDesc(s, k);
                break;
            }
            case 6: {
                char sid[MAX_ID_LEN];
                printf("请输入学号: "); scanf("%s", sid);
                score_avgByStudent(s, sid);
                break;
            }
            case 7: {
                char cid[MAX_ID_LEN];
                printf("请输入课程号: "); scanf("%s", cid);
                score_avgByCourse(s, cid);
                break;
            }
            case 0: return;
            default: printf("❌ 无效选项\n");
        }
    }
}
```

#### 10.5.2 ranking_service_menu（在 RankingService.c）

```c
void ranking_service_menu(RankingService* r) {
    int choice;
    while (true) {
        printf("\n========== 排行榜 ==========\n");
        printf(" 1. 学生绩点 TopK\n");
        printf(" 2. 教师授课评分 TopK\n");
        printf(" 0. 返回主菜单\n");
        printf("==============================\n");
        printf("请输入选项: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: {
                int k; printf("请输入 K 值: "); scanf("%d", &k);
                ranking_topKStudents(r->studentSvc, k);
                break;
            }
            case 2: {
                int k; printf("请输入 K 值: "); scanf("%d", &k);
                ranking_topKTeachers(r->teacherSvc, k);
                break;
            }
            case 0: return;
            default: printf("❌ 无效选项\n");
        }
    }
}
```

### 10.6 ScoreService + RankingService 接口定义

```c
// ----- ScoreService -----
// ScoreList 是与 CourseList / TeacherHashTable 同模式的并列容器，存 Score 而非泛型
typedef struct {
    Score* data;
    int size;
    int capacity;
} ScoreList;

ScoreList* scorelist_create(void);
void       scorelist_destroy(ScoreList* l);
bool       scorelist_insert(ScoreList* l, Score sc);
bool       scorelist_remove(ScoreList* l,
                            const char* studentId, const char* courseId);
Score*     scorelist_find(ScoreList* l,
                          const char* studentId, const char* courseId);
Score*     scorelist_at(ScoreList* l, int index);
int        scorelist_size(ScoreList* l);

typedef struct {
    ScoreList*        scores;
    StudentService*   studentSvc;     // 引用全局学生服务（用于按学号查姓名）
    CourseService*    courseSvc;      // 引用全局课程服务（用于按课程号查课程名）
} ScoreService;

ScoreService* score_service_create(StudentService* ss, CourseService* cs);
void          score_service_destroy(ScoreService* s);

bool score_add(ScoreService* s, Score sc);
bool score_remove(ScoreService* s,
                  const char* studentId, const char* courseId);
void score_findByStudent(ScoreService* s, const char* studentId);
void score_findByCourse(ScoreService* s, const char* courseId);
void score_sortByScoreDesc(ScoreService* s, int k);
void score_avgByStudent(ScoreService* s, const char* studentId);
void score_avgByCourse(ScoreService* s, const char* courseId);

void score_service_menu(ScoreService* s);   // 见 §10.5.1

// ----- RankingService -----
typedef struct {
    StudentService* studentSvc;   // 引用（不强拥有）
    TeacherService* teacherSvc;
} RankingService;

RankingService* ranking_service_create(StudentService* ss, TeacherService* ts);
void            ranking_service_destroy(RankingService* r);

void ranking_topKStudents(StudentService* ss, int k);
void ranking_topKTeachers(TeacherService* ts, int k);

void ranking_service_menu(RankingService* r);   // 见 §10.5.2
```

---

## 十一、协作约定

### 11.1 文件依赖关系

```
entity/   <- 无依赖（最底层）
common.h  <- 无依赖
ds/       <- 仅依赖 common.h + entity/
service/  <- 依赖 ds/ 和 entity/
ui/       <- 依赖 service/
main.c    <- 依赖 ui/ 和 data/
```

### 11.2 全局对象声明

所有全局 Service 句柄在 `main.c` 中定义：

```c
// main.c
TeacherService* g_teacherService = NULL;
// ...

// 其他 .c 文件
extern TeacherService* g_teacherService;
```

