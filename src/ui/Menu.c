/*
负责人：陆奕炜
*/
#include "Menu.h"

#include "../service/TeacherService.h"
#include "../service/StudentService.h"
#include "../service/CourseService.h"
#include "../service/ScoreService.h"
#include "../service/SearchService.h"
#include "../service/RankingService.h"
#include "../service/NoticeService.h"
#include "../service/OperationHistory.h"

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
            default: printf("无效选项\n");
        }
    }
}
