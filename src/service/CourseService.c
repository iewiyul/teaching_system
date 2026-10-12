/*
负责人：陆奕炜
*/
#include "CourseService.h"
/* course_queryStudentCourses 里要调 scorelist_find、读 ScoreService 内部
   ScoreList*；CourseService.h 只前向声明 ScoreService*，故 .c 里 include
   ScoreService.h。ScoreService.h 不反向 include CourseService.h，安全。 */
#include "../entity/Score.h"
#include "../service/ScoreService.h"

CourseList *courselist_create(void) {
    CourseList *newNode = (CourseList *)malloc(sizeof(CourseList));
    if (!newNode)
        return NULL;
    memset(newNode, 0, sizeof(CourseList));
    newNode->capacity = 100;
    Course *c = (Course *)malloc(100 * sizeof(Course));
    if (!c) {
        free(newNode);
        return NULL;
    }
    newNode->data = c;
    return newNode;
}
void courselist_destroy(CourseList *l) {
    if (!l)
        return;
    if (l->data)
        free(l->data);
    free(l);
}

bool courselist_insert(CourseList *l, Course c) {
    if (l->size == l->capacity) {
        Course *newNode;
        if (!(newNode = realloc(l->data, (l->capacity * 2) * sizeof(Course))))
            return false;
        l->data = newNode;
        l->data[l->capacity] = c;
        l->capacity++;
    } else {
        l->data[l->size] = c;
    }
    l->size++;
    return true;
}

bool courselist_remove(CourseList *l, const char *id) {
    bool has = false;
    for (int i = 0; i < l->size; i++) {
        if (0 == strcmp(l->data[i].id, id)) {
            has = true;
            l->size--;
            for (int j = i; j < l->size; j++) {
                l->data[j] = (l->data[j + 1]);
            }
            break;
        }
    }
    if (!has)
        return false;
    return true;
}
bool courselist_update(CourseList *l, const char *id, Course c) {
    bool has = false;
    for (int i = 0; i < l->size; i++) {
        if (0 == strcmp(l->data[i].id, id)) {
            l->data[i] = c;
            has = true;
            break;
        }
    }
    if (!has)
        return false;
    return true;
}
Course *courselist_find(CourseList *l, const char *id) {
    for (int i = 0; i < l->size; i++) {
        if (0 == strcmp(l->data[i].id, id)) {
            return l->data + i;
        }
    }
    return NULL;
}
Course *courselist_at(CourseList *l, int index) {
    if (index >= l->size) {
        return NULL;
    } else {
        return l->data + index;
    }
}
int courselist_size(CourseList *l) { return l->size; }

CourseService *course_service_create(SeqList *teachers, LinkedList *students,
                                     ScoreService *scores) {
    CourseService *newNode = (CourseService *)malloc(sizeof(CourseService));
    if (!newNode) {
        return NULL;
    }
    memset(newNode, 0, sizeof(CourseService));
    newNode->teachersRef = teachers;
    newNode->studentsRef = students;
    newNode->scoresRef = scores;
    newNode->courses = courselist_create();
    newNode->courseGraph = graph_create();
    if (!newNode->courses || !newNode->courseGraph) {
        if (newNode->courses) {
            courselist_destroy(newNode->courses);
        }
        if (newNode->courseGraph) {
            graph_destroy(newNode->courseGraph);
        }
        free(newNode);
        return NULL;
    }
    return newNode;
}
void course_service_destroy(CourseService *s) {
    if (!s)
        return;
    courselist_destroy(s->courses);
    graph_destroy(s->courseGraph);
    free(s);
}

bool course_add(CourseService *s, Course c) {
    if (!s)
        return false;
    if (!courselist_insert(s->courses, c))
        return false;
    if (!graph_addVertex(s->courseGraph, c.id)) {
        courselist_remove(s->courses, c.id);
        return false;
    }
    history_record("add", "Course", c.id, "添加一门课程");
    return true;
}
bool course_remove(CourseService *s, const char *id) {
    if (!s)
        return false;
    printf("确认删除%s\n这门课程吗，确认输入y\n", id);
    char queren;
    scanf(" %c", &queren);
    if (queren != 'y')
        return false;
    if (!courselist_find(s->courses, id))
        return false;
    if (!graph_removeVertex(s->courseGraph, id)) {
        return false;
    }
    if (!courselist_remove(s->courses, id))
        return false;
    history_record("remove", "Course", id, "删除课程");
    return true;
}
bool course_update(CourseService *s, const char *id, Course c) {
    if (!s)
        return false;
    if (strcmp(id, c.id) != 0)
        return false;
    if (!courselist_update(s->courses, id, c)) {
        return false;
    }
    history_record("update", "Course", id, "更新课程信息");
    return true;
}
/* 私有辅助：SeqList<Teacher> 中按 id 线性找 Teacher*
   成员 C 尚未交付 seqlist_find，先借 seqlist_at 走一遍；
   seqlist_find 到位后可替成直接调用 */
static Teacher *find_teacher_ref(SeqList *sl, const char *id) {
    if (!sl || !id)
        return NULL;
    int n = seqlist_size(sl);
    for (int i = 0; i < n; i++) {
        Teacher *t = seqlist_at(sl, i);
        if (t && strcmp(t->id, id) == 0)
            return t;
    }
    return NULL;
}

/* 私有辅助：LinkedList<Student> 中按 id 线性找 Student*（直接走 head 链）
   linkedlist_find 到位后可替成直接调用 */
static Student *find_student_ref(LinkedList *ll, const char *id) {
    if (!ll || !id)
        return NULL;
    for (ListNode *cur = ll->head; cur; cur = cur->next) {
        if (strcmp(cur->data.id, id) == 0)
            return &cur->data;
    }
    return NULL;
}

Course *course_queryById(CourseService *s, const char *id) {
    if (!s || !id)
        return NULL;
    Course *c = courselist_find(s->courses, id);
    if (!c) {
        printf("未找到课程号 %s\n", id);
        return NULL;
    }
    char ids[128][MAX_ID_LEN];
    int cnt = 0;

    printf("=== 课程 %s 详情 ===\n", c->id);
    printf("课程名 : %s\n", c->name);
    printf("学分   : %.1f\n", c->credit);
    printf("学时   : %d\n", c->hours);
    printf("学期   : %s\n", c->semester);

    graph_getTeachersOfCourse(s->courseGraph, c->id, ids, &cnt);
    printf("--- 任课教师（共 %d 人）---\n", cnt);
    for (int i = 0; i < cnt; i++) {
        Teacher *t = find_teacher_ref(s->teachersRef, ids[i]);
        printf("  %s  %s\n", ids[i], t ? t->name : "(姓名未登记)");
    }

    graph_getStudentsOfCourse(s->courseGraph, c->id, ids, &cnt);
    int show = cnt < 10 ? cnt : 10;
    printf("--- 选课学生（前 %d / 共 %d 人）---\n", show, cnt);
    for (int i = 0; i < show; i++) {
        Student *st = find_student_ref(s->studentsRef, ids[i]);
        printf("  %s  %s\n", ids[i], st ? st->name : "(姓名未登记)");
    }
    if (cnt > 10)
        printf("（还有 %d 名学生未显示）\n", cnt - 10);
    return c;
}

void course_listAll(CourseService *s) {
    if (!s)
        return;
    int n = courselist_size(s->courses);
    if (n == 0) {
        printf("暂无课程\n");
        return;
    }
    printf("=== 课程列表（共 %d 门）===\n", n);
    printf("%-8s %-30s %5s %6s %-12s\n", "课程号", "课程名", "学分", "学时",
           "学期");
    for (int i = 0; i < n; i++) {
        Course *c = courselist_at(s->courses, i);
        if (!c)
            continue;
        printf("%-8s %-30s %5.1f %6d %-12s\n", c->id, c->name, c->credit,
               c->hours, c->semester);
    }
}

bool course_assignTeacher(CourseService *s, const char *courseId,
                          const char *teacherId, double rating) {
    if (!s)
        return false;
    if (!courselist_find(s->courses, courseId)) {
        printf("课程不存在：%s\n", courseId);
        return false;
    }
    if (!find_teacher_ref(s->teachersRef, teacherId)) { // 复用刚才的辅助
        printf("教师不存在：%s\n", teacherId);
        return false;
    }
    if (!graph_addEdge(s->courseGraph, teacherId, courseId, EDGE_TEACH,
                       rating)) {
        return false;
    }
    history_record("add", "Edge", courseId, "建立老师授课课程的关系");
    return true;
}
bool course_unassignTeacher(CourseService *s, const char *courseId,
                            const char *teacherId) {
    if (!s)
        return false;
    printf("确认解除 %s 任课 %s 吗？(y/n)\n", teacherId, courseId);
    char queren;
    if (scanf(" %c", &queren) != 1 || queren != 'y')
        return false;

    if (!graph_removeEdge(s->courseGraph, teacherId, courseId, EDGE_TEACH)) {
        return false;
    }
    history_record("remove", "Edge", courseId, "解除老师授课课程的关系");
    return true;
}
void course_queryTeacherCourses(CourseService *s, const char *teacherId) {
    if (!s || !teacherId) return;
    Teacher *t = find_teacher_ref(s->teachersRef, teacherId);
    if (!t) {
        printf("❌ 教师不存在：%s\n", teacherId);
        return;
    }
    char ids[128][MAX_ID_LEN];
    int  cnt = 0;
    graph_getCoursesOfTeacher(s->courseGraph, teacherId, ids, &cnt);
    printf("=== 教师 %s（%s）的任课（共 %d 门）===\n",
           teacherId, t->name, cnt);
    if (cnt == 0) { printf("（暂无任课）\n"); return; }
    printf("%-8s %-30s %5s %6s\n", "课程号", "课程名", "学分", "学时");
    for (int i = 0; i < cnt; i++) {
        Course *c = courselist_find(s->courses, ids[i]);
        if (!c) {
            printf("  %s  (课程已删除)\n", ids[i]);
            continue;
        }
        printf("%-8s %-30s %5.1f %6d\n",
               c->id, c->name, c->credit, c->hours);
    }
}

void course_queryCourseTeachers(CourseService *s, const char *courseId) {
    if (!s || !courseId) return;
    Course *c = courselist_find(s->courses, courseId);
    if (!c) {
        printf("❌ 课程不存在：%s\n", courseId);
        return;
    }
    char ids[128][MAX_ID_LEN];
    int  cnt = 0;
    graph_getTeachersOfCourse(s->courseGraph, courseId, ids, &cnt);
    printf("=== 课程 %s（%s）的任课教师（共 %d 人）===\n",
           c->id, c->name, cnt);
    if (cnt == 0) { printf("（暂无任课教师）\n"); return; }
    for (int i = 0; i < cnt; i++) {
        Teacher *t = find_teacher_ref(s->teachersRef, ids[i]);
        printf("  %s  %s\n", ids[i], t ? t->name : "(姓名未登记)");
    }
}

bool course_addStudent(CourseService *s, const char *courseId,
                       const char *studentId) {
    if (!s || !courseId || !studentId) return false;
    if (!courselist_find(s->courses, courseId)) {
        printf("❌ 课程不存在：%s\n", courseId);
        return false;
    }
    if (!find_student_ref(s->studentsRef, studentId)) {
        printf("❌ 学生不存在：%s\n", studentId);
        return false;
    }
    /* EDGE_TAKE 方向：from = 学生, to = 课程 */
    if (!graph_addEdge(s->courseGraph, studentId, courseId,
                       EDGE_TAKE, 1.0)) {
        return false;
    }
    history_record("add", "Edge", courseId, "建立学生选课关系");
    return true;
}

bool course_removeStudent(CourseService *s, const char *courseId,
                          const char *studentId) {
    if (!s || !courseId || !studentId) return false;
    printf("确认解除 %s 退课 %s 吗？(y/n)\n", studentId, courseId);
    char queren;
    if (scanf(" %c", &queren) != 1 || queren != 'y') return false;
    if (!graph_removeEdge(s->courseGraph, studentId, courseId, EDGE_TAKE)) {
        return false;
    }
    history_record("remove", "Edge", courseId, "解除学生选课关系");
    return true;
}

void course_queryStudentCourses(CourseService *s, const char *studentId) {
    if (!s || !studentId) return;
    Student *st = find_student_ref(s->studentsRef, studentId);
    if (!st) {
        printf("❌ 学生不存在：%s\n", studentId);
        return;
    }
    char ids[128][MAX_ID_LEN];
    int  cnt = 0;
    graph_getCoursesOfStudent(s->courseGraph, studentId, ids, &cnt);
    printf("=== 学生 %s（%s）的选课（共 %d 门）===\n",
           studentId, st->name, cnt);
    if (cnt == 0) { printf("（暂无选课）\n"); return; }
    printf("%-8s %-30s %10s\n", "课程号", "课程名", "成绩");
    for (int i = 0; i < cnt; i++) {
        Course *c = courselist_find(s->courses, ids[i]);
        /* scorelist_find 暂时还是 TODO（成员 E 未交付），先降级显示 */
        Score *sc = (s->scoresRef && s->scoresRef->scores)
                    ? scorelist_find(s->scoresRef->scores,
                                     studentId, ids[i])
                    : NULL;
        printf("%-8s %-30s ", ids[i], c ? c->name : "(课程已删除)");
        if (sc) printf("%.1f\n", sc->value);
        else    printf("(暂无成绩)\n");
    }
}

void course_queryCourseStudents(CourseService *s, const char *courseId) {
    if (!s || !courseId) return;
    Course *c = courselist_find(s->courses, courseId);
    if (!c) {
        printf("❌ 课程不存在：%s\n", courseId);
        return;
    }
    char ids[128][MAX_ID_LEN];
    int  cnt = 0;
    graph_getStudentsOfCourse(s->courseGraph, courseId, ids, &cnt);
    printf("=== 课程 %s（%s）的选课学生（共 %d 人）===\n",
           c->id, c->name, cnt);
    if (cnt == 0) { printf("（暂无学生）\n"); return; }
    int show = cnt < 10 ? cnt : 10;
    printf("%-12s %-10s %6s %5s\n", "学号", "姓名", "绩点", "年级");
    for (int i = 0; i < show; i++) {
        Student *st = find_student_ref(s->studentsRef, ids[i]);
        if (!st) {
            printf("%-12s (姓名未登记)\n", ids[i]);
            continue;
        }
        printf("%-12s %-10s %6.2f %5d\n",
               st->id, st->name, st->gpa, st->grade);
    }
    if (cnt > 10) printf("（还有 %d 名学生未显示）\n", cnt - 10);
}

Graph *course_service_getGraph(CourseService *s) {
    if (!s) return NULL;
    return s->courseGraph;
}

CourseList *course_service_getList(CourseService *s) {
    if (!s) return NULL;
    return s->courses;
}

void course_service_menu(CourseService *s) {
    if (!s) return;
    int    choice;
    char   id[MAX_ID_LEN],  id2[MAX_ID_LEN];
    char   semester[20];
    double rating, credit;
    int    hours;

    while (1) {
        printf("\n========== 课程管理 ==========\n");
        printf(" 1. 新增课程\n");
        printf(" 2. 删除课程\n");
        printf(" 3. 修改课程\n");
        printf(" 4. 按课程号查询\n");
        printf(" 5. 列出全部课程\n");
        printf(" 6. 添加任课教师\n");
        printf(" 7. 删除任课教师\n");
        printf(" 8. 查询某教师的所有课程\n");
        printf(" 9. 查询某课程的所有教师\n");
        printf("10. 添加学生选课\n");
        printf("11. 删除学生选课\n");
        printf("12. 查询某学生的所有课程\n");
        printf("13. 查询某课程的所有学生\n");
        printf(" 0. 返回主菜单\n");
        printf("==============================\n");
        printf("请输入选项: ");
        if (scanf("%d", &choice) != 1) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {}
            printf("❌ 请输入数字选项\n");
            continue;
        }
        if (choice == 0) return;

        switch (choice) {
        case 1: {                                 /* 新增课程 */
            Course c; memset(&c, 0, sizeof(c));
            printf("--- 新增课程 ---\n");
            printf("课程号: ");   scanf("%19s", c.id);
            printf("课程名: ");   scanf("%49s", c.name);
            printf("学分: ");     scanf("%lf",  &credit); c.credit = credit;
            printf("学时: ");     scanf("%d",   &hours);   c.hours  = hours;
            printf("学期: ");     scanf("%19s", semester);
            snprintf(c.semester, sizeof(c.semester), "%s", semester);
            if (course_add(s, c)) printf("✅ 课程添加成功！\n");
            break;
        }
        case 2: {                                 /* 删除课程 */
            printf("请输入要删除的课程号: ");
            scanf("%19s", id);
            if (course_remove(s, id)) printf("✅ 课程已删除\n");
            break;
        }
        case 3: {                                 /* 修改课程 */
            Course c; memset(&c, 0, sizeof(c));
            printf("请输入要修改的课程号: ");
            scanf("%19s", id);
            snprintf(c.id, sizeof(c.id), "%s", id);   /* course_update 要求 c.id==id */
            printf("新课程名: "); scanf("%49s", c.name);
            printf("新学分: ");   scanf("%lf",  &credit); c.credit = credit;
            printf("新学时: ");   scanf("%d",   &hours);   c.hours  = hours;
            printf("新学期: ");   scanf("%19s", semester);
            snprintf(c.semester, sizeof(c.semester), "%s", semester);
            if (course_update(s, id, c)) printf("✅ 课程修改成功！\n");
            break;
        }
        case 4:                                   /* 按课程号查询 */
            printf("请输入课程号: ");
            scanf("%19s", id);
            course_queryById(s, id);
            break;
        case 5:                                   /* 列出全部课程 */
            course_listAll(s);
            break;
        case 6: {                                 /* 添加任课教师 */
            printf("--- 添加任课教师 ---\n");
            printf("课程号: ");     scanf("%19s", id);
            printf("教师工号: ");   scanf("%19s", id2);
            printf("授课评分(0~5): ");
            scanf("%lf", &rating);
            if (course_assignTeacher(s, id, id2, rating))
                printf("✅ 任课关系已建立：%s ← %s（评分 %.1f）\n",
                       id, id2, rating);
            break;
        }
        case 7: {                                 /* 删除任课教师 */
            printf("请输入课程号: ");   scanf("%19s", id);
            printf("请输入教师工号: "); scanf("%19s", id2);
            if (course_unassignTeacher(s, id, id2))
                printf("✅ 已解除 %s 在 %s 的任课关系\n", id2, id);
            break;
        }
        case 8:                                   /* 查询某教师的所有课程 */
            printf("请输入教师工号: ");
            scanf("%19s", id);
            course_queryTeacherCourses(s, id);
            break;
        case 9:                                   /* 查询某课程的所有教师 */
            printf("请输入课程号: ");
            scanf("%19s", id);
            course_queryCourseTeachers(s, id);
            break;
        case 10: {                                /* 添加学生选课 */
            printf("--- 添加学生选课 ---\n");
            printf("课程号: "); scanf("%19s", id);
            printf("学号: ");   scanf("%19s", id2);
            if (course_addStudent(s, id, id2))
                printf("✅ 选课成功：%s ← %s\n", id, id2);
            break;
        }
        case 11: {                                /* 删除学生选课 */
            printf("请输入课程号: "); scanf("%19s", id);
            printf("请输入学号: ");   scanf("%19s", id2);
            if (course_removeStudent(s, id, id2))
                printf("✅ 已退课：%s ← %s\n", id, id2);
            break;
        }
        case 12:                                  /* 查询某学生的所有课程 */
            printf("请输入学号: ");
            scanf("%19s", id);
            course_queryStudentCourses(s, id);
            break;
        case 13:                                  /* 查询某课程的所有学生 */
            printf("请输入课程号: ");
            scanf("%19s", id);
            course_queryCourseStudents(s, id);
            break;
        default:
            printf("❌ 无效选项 %d\n", choice);
            break;
        }
    }
}
