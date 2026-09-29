/*
负责人：陆奕炜
*/
#include "common.h"

#include "data/seed.h"
#include "service/CourseService.h"
#include "service/NoticeService.h"
#include "service/OperationHistory.h"
#include "service/RankingService.h"
#include "service/ScoreService.h"
#include "service/SearchService.h"
#include "service/StudentService.h"
#include "service/TeacherService.h"
#include "ui/Menu.h"

#include <windows.h>
/* 全局 Service 句柄定义（其他 .c 文件 extern 引用） */
TeacherService *g_teacherService = NULL;
StudentService *g_studentService = NULL;
CourseService *g_courseService = NULL;
ScoreService *g_scoreService = NULL;
NoticeService *g_noticeService = NULL;
SearchService *g_searchService = NULL;
RankingService *g_rankingService = NULL;
OperationHistory *g_operationHistory = NULL;

int main(void) {
    SetConsoleOutputCP(CP_UTF8);
    /* 1. create 各 Service（顺序见 CLAUDE.md 层依赖图）
     *    注意：CourseService 与 ScoreService 互相引用，按 README §6.7 顺序让
     * course 先创建， 此时 ScoreService 还没好，CourseService 的 scores
     * 字段暂时传 NULL 占位。 CourseService.c 是 stub 不访问
     * scores，故编译/运行不会崩； 待 B/C/D/E 实现 CourseService.c 时需自行处理
     * NULL scores（延迟访问 / setter 后填） */
    g_teacherService = teacher_service_create();
    g_studentService = student_service_create();
    g_courseService = course_service_create(NULL, NULL, NULL);
    g_scoreService = score_service_create(g_studentService, g_courseService);
    g_noticeService = notice_service_create();
    g_searchService = search_service_create(g_studentService, g_teacherService);
    g_rankingService =
        ranking_service_create(g_studentService, g_teacherService);

    /* 2. OperationHistory 必须放在 init_seed_data 之前：
     *    所有 add/remove/update 都会调 history_record，往
     * g_operationHistory->stack 压栈。 （README §6.7 模板把它放最后是错的——那样
     * seed 阶段的操作不会被记录。） */
    g_operationHistory = operation_history_create();

    /* 3. 种子数据（README §6.5）：5 课程 → 15 教师 → 350 学生 → 15 任课边 →
     * 选课边 → rebuildIndexes */
    init_seed_data();

    /* 4. 进入主菜单（README §6.8） */
    menu_run();

    /* 5. 逆序 destroy（与 create 反向） */
    ranking_service_destroy(g_rankingService);
    search_service_destroy(g_searchService);
    notice_service_destroy(g_noticeService);
    score_service_destroy(g_scoreService);
    course_service_destroy(g_courseService);
    student_service_destroy(g_studentService);
    teacher_service_destroy(g_teacherService);
    operation_history_destroy(g_operationHistory);

    return 0;
}