/*
负责人：陆奕炜
*/
#include "common.h"
#include "entity/Course.h"
#include "entity/Student.h"
#include "entity/Teacher.h"
#include "service/CourseService.h"
#include "service/SearchService.h"
#include "service/StudentService.h"
#include "service/TeacherService.h"
#include <time.h>

extern TeacherService *g_teacherService;
extern StudentService *g_studentService;
extern CourseService *g_courseService;
extern SearchService *g_searchService;

void init_seed_data(void) {
    srand((unsigned)time(NULL));

    printf("[种子数据加载中...]\n");

    /* 1. 5 门课程 */
    Course courses[] = {
        {"C001", "数据结构", 4, 64, "2024秋"},
        {"C002", "算法分析", 3, 48, "2024秋"},
        {"C003", "数据库原理", 4, 64, "2024秋"},
        {"C004", "操作系统", 4, 64, "2025春"},
        {"C005", "计算机网络", 3, 48, "2025春"},
    };
    for (int i = 0; i < 5; i++)
        course_add(g_courseService, courses[i]);
    printf("  课程数：%d\n", 5);

    /* 2. 15 名教师 */
    const char *tnames[] = {"张伟", "李娜", "王芳", "刘洋", "陈静",
                            "杨帆", "周强", "吴敏", "徐磊", "孙丽",
                            "马超", "朱琳", "胡军", "林峰", "何婷"};
    const char *titles[] = {"教授", "副教授", "讲师"};
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

    /* 3. 350 名学生 */
    const char *snames[] = {"赵一", "钱二", "孙三", "李四", "周五",
                            "吴六", "郑七", "王八", "冯九", "陈十"};
    const char *majors[] = {"计算机", "软件工程", "人工智能", "信息安全"};
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

    /* 4. 任课关系 */
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

    /* 5. 选课关系 */
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

    /* 6. 重建查找索引（必须最后调，否则 §9 SearchService 功能 1-4 跑不通） */
    printf("  构建查找索引...\n");
    search_service_rebuildIndexes(g_searchService);

    printf("[本地数据生成完成]\n\n");
}