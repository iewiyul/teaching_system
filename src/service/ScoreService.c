/*
负责人：成员 E，功能是实现成绩管理和成绩排序
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "ScoreService.h"
#include "StudentService.h"
#include "CourseService.h"

//socre 堆排序
static void score_sift_down(Score* a, int i, int n,bool desc) {
    while (1) {
        int l = 2 * i + 1;//左孩子下标
        int r = 2 * i + 2;//右孩子下标
        int best = i;//假设此时 i 最应该在上面
        if (l < n && desc ? a[l].value > a[best].value : a[l].value < a[best].value)//若左孩子存在，将左孩子与父亲作比较
        {
            best = l;
        }
        if (r < n && desc ? a[r].value > a[best].value : a[r].value < a[best].value)//若右孩子存在，将右孩子与父亲作比较
        {
            best = r;
        }
        if (best == i) {
            break;
        }
        Score t = a[i];
        a[i] = a[best];
        a[best] = t;//和更优先交换位置
        i = best;//继续下沉
    }
}
//堆排序，降序
static void heapsort_scores_desc(Score*arr,int n) {
    for (int i = n / 2 - 1;i >= 0;i--) {
        score_sift_down(arr,i,n,true);//建大顶堆
    }
    for (int i = n - 1;i > 0;i--) {//将堆顶元素，最大分数与末尾元素交换，然后重新调整
        Score t = arr[0];
        arr[0] = arr[i];
        arr[i] = t;
        score_sift_down(arr,0,i,true);//剩余下沉，继续调整为大顶堆
    }
}

//动态数组
//创建一个空的成绩列表
ScoreList* scorelist_create(void) {
    ScoreList* l = (ScoreList*)malloc(sizeof(ScoreList));
    if (!l)return NULL;
    l->capacity = 16;
    l->size = 0;
    l->data = (Score*)malloc(sizeof(Score) * l->capacity);//申请16个Score的空间
    if (!l->data) { free(l);return NULL; }
    return l;
}
//销毁成绩列表
void  scorelist_destroy(ScoreList* l)  { 
    if (!l)return;
    free(l->data);
    free(l);
}
//插入成绩
bool  scorelist_insert(ScoreList* l, Score sc) { 
    if (l->size == l->capacity) {
        int nc = l->capacity * 2;
        Score* nd = (Score*)realloc(l->data, sizeof(Score) * nc);//重新分配内存，扩容
        if (!nd)return false;
        l->data = nd;
        l->capacity = nc;
    }
    l->data[l->size++] = sc;//将成绩插入到数组末尾
    return true;
}
//删除某学生的成绩
bool scorelist_remove(ScoreList* l, const char* studentId, const char* courseId) { 
    for (int i = 0;i < l->size;i++) {
        if (strcmp(l->data[i].studentId,studentId)==0 && strcmp(l->data[i].courseId,courseId) == 0) {//找到要删除的成绩
            l->data[i] = l->data[l->size - 1];//把最后一个成绩移动到当前位置
            l->size--;//成绩数量-1
            return true;
        }
    }
    return false;
}
//查找某学生的成绩
Score*     scorelist_find(ScoreList* l, const char* studentId, const char* courseId)   { 
    for (int i = 0;i < l->size;i++) {//遍历成绩列表
        if (strcmp(l->data[i].studentId, studentId) == 0 && strcmp(l->data[i].courseId, courseId) == 0)//学号和课程号都匹配
            return &l->data[i];//返回成绩的地址
    }
    return NULL;//没有找到
}
//获取指定索引处的成绩
Score*     scorelist_at(ScoreList* l, int index)  { 
    if (index<0 || index>= l->size) {
        return NULL;
    }
    return &l->data[index];
}
//获取成绩列表的大小
int scorelist_size(ScoreList* l)  { 
    return 1 ? l->size : 0;
}

//创建成绩服务-系统启动时调用
ScoreService* score_service_create(StudentService* ss, CourseService* cs) {
    ScoreService* s = (ScoreService*)malloc(sizeof(ScoreService));
    if (!s) return NULL;
    s->scores = scorelist_create();//创建空的成绩列表
    s->studentSvc = ss;//查名字
    s->courseSvc = cs;//查课程名
    return s;
}
//销毁成绩服务
void   score_service_destroy(ScoreService* s) { 
    if (!s)return;
    scorelist_destroy(s->scores);
    free(s);
}
//新增成绩-对应菜单1
bool   score_add(ScoreService* s, Score sc) {
    if (scorelist_find(s->scores,sc.studentId,sc.courseId)) {//判断该学生该课程是否已有成绩记录
        printf("该学生该课程已有成绩记录！\n");
        return false;
    }
    if (!scorelist_insert(s->scores, sc))return false;//插入成绩
    history_record("add","Score",sc.studentId,"新增成绩");//记录操作历史
    printf("成绩记录已添加\n");
    return true;
}
//删除成绩-对应菜单2
bool   score_remove(ScoreService* s, const char* studentId, const char* courseId){ 
    if (!scorelist_remove(s->scores,studentId,courseId)) {
        printf("未找到该学生的成绩记录！\n");
        return false;
    }
    history_record("remove","Score",studentId,"删除成绩");
    printf("成绩记录已删除\n");
    return true;
}
//按照学号查询成绩-对应菜单3
void   score_findByStudent(ScoreService* s, const char* studentId)  { 
    Student* stu = student_queryById(s->studentSvc,studentId);//查找学生信息
    if (!stu) {
        printf("未找到该学生\n");
        return;
    }
    printf("---%s %s 的成绩 ---\n",stu->id,stu->name);
    printf("课程号\t课程名\t\t分数\t类型\n");
    double sum = 0;
    int cnt = 0;
    for (int i = 0; i < scorelist_size(s->scores); i++) {//遍历成绩列表
        Score* sc = scorelist_at(s->scores,i);//获取第i条成绩
        if (strcmp(sc->studentId, studentId) != 0)continue;
        Course* c = course_queryById(s->courseSvc,sc->courseId);//查找课程信息
        printf("%s\t%s\t\t%.1f\t%s\n",sc->courseId,c? c->name : "?",sc->value,sc->examType);//打印成绩信息
        sum += sc->value;
        cnt++;
    }
    if (cnt > 0)printf("平均分:%.1f\n", sum / cnt);//计算平均分
    else printf("(无成绩记录)\n");
}
//按照课程号查询成绩-对应菜单4
void   score_findByCourse(ScoreService* s, const char* courseId)  { 
    Course* c = course_queryById(s->courseSvc,courseId);//查找课程信息
    if (!c) {
        printf("未找到该课程\n");
        return;
    }
    double sum = 0;
    int cnt = 0;
    for (int i = 0; i < scorelist_size(s->scores); i++) {
        Score* sc = scorelist_at(s->scores, i);
        if (strcmp(sc->courseId, courseId) == 0) {
            sum += sc->value;
            cnt++;
        }
    }
    printf("--- %s %s 成绩 (共 %d 条) ---\n",c->id,c->name,cnt);
    if (cnt == 0)return;
    printf("学号\t\t姓名\t分数\n");
    for (int i = 0; i < scorelist_size(s->scores); i++) {
        Score* sc = scorelist_at(s->scores, i);
        if (strcmp(sc->courseId, courseId) != 0)continue;
        Student* stu = student_queryById(s->studentSvc,sc->studentId);
        printf("%s\t%s\t%.1f\n",sc->studentId,stu ? stu->name: "?",sc->value);
    }
    printf("平均分:%.1f\n",sum/cnt);
}
//分数与绩点的相互转换
static double local_score_to_gpa(double score) {
    if (score >= 90)return 4.0;
    if (score >= 85)return 3.7;
    if (score >= 82)return 3.3;
    if (score >= 78)return 3.0;
    if (score >= 75)return 2.7;
    if (score >= 72)return 2.3;
    if (score >= 68)return 2.0;
    if (score >= 66)return 1.7;
    if (score >= 64)return 1.5;
    if (score >= 60)return 1.0;
    if (score < 60.0)return 0.0;
    return 0.0;
}
//按照分数排序-对应菜单5
void   score_sortByScoreDesc(ScoreService* s, int k)   { 
    int n = scorelist_size(s->scores);//获取成绩数量
    if (n == 0) {
        printf("暂无成绩记录\n");
        return;
    }
    Score* work = (Score*)malloc(sizeof(Score)*n);//创建临时工作数组
    if (!work)return;
    clock_t t0 = clock();//记录开始时间
    for (int r = 0; r < 100; r++) {
        for (int i = 0; i < n; i++) {
            work[i] = s->scores->data[i];//复制成绩数据到工作数组
        }
        heapsort_scores_desc(work,n);//对工作数组进行堆排序
    }
    clock_t t1 = clock();//记录结束时间
    double us = (double)(t1 - t0) * 1e6 / CLOCKS_PER_SEC / 100;//计算平均耗时
    printf("[堆排序]数据规模 %d 条\n",n);
    printf("--- 全部成绩按分数降序(共 %d 条)---\n",k);
    printf("学号\t\t课程号\t分数\n");
    int shown = (k<n) ? k : n;
    for (int i = 0; i < shown;i++) {
        printf("%s\t%s\t%.1f\n", work[i].studentId, work[i].courseId, work[i].value);//打印前k条成绩
    }
    printf("堆排序耗时: %.2f us (100次取平均)\n",us);//打印耗时结果
    free(work);
}
//计算学生的平均分-对应菜单6
void   score_avgByStudent(ScoreService* s, const char* studentId){ 
    Student* stu = student_queryById(s->studentSvc,studentId);
    if (!stu) {
        printf("未找到该学生\n");
        return;
    }
    double sum = 0;
    int cnt = 0;
    for (int i = 0;i < scorelist_size(s->scores);i++) {
        Score* sc = scorelist_at(s->scores,i);
        if (strcmp(sc->studentId,studentId)==0) {
            sum += sc->value;
            cnt++;
        }
    }
    double avg = cnt ? sum / cnt : 0.0;
    printf("-- %s %s 成绩统计 ---\n",stu->id,stu->name);
    printf("课程数: %d\n总分: %.1f\n平均分: %.1f\n",cnt,sum,avg);
    printf("对应平均绩点: %.1f\n",local_score_to_gpa(avg));
}
//计算课程的平均分-对应菜单7
void   score_avgByCourse(ScoreService* s, const char* courseId){ 
    Course* c = course_queryById(s->courseSvc, courseId);
    if (!c) {
        printf("未找到该课程\n");
        return;
    }
    double sum = 0;
    int cnt = 0;
    for (int i = 0; i < scorelist_size(s->scores); i++) {
        Score* sc = scorelist_at(s->scores, i);
        if (strcmp(sc->courseId, courseId) == 0) {
            sum += sc->value;
            cnt++;
        }
    }
    printf("-- %s %s 成绩统计 ---\n", c->id, c->name);
    printf("学生数: %d\n总分: %.1f\n平均分: %.1f\n", cnt, sum, cnt ? sum / cnt : 0.0);
}
//成绩菜单界面
void   score_service_menu(ScoreService* s) {
    int choice;
    while (1) {
        printf("\n========== 成绩管理 ==========\n");
        printf(" 1. 新增成绩记录\n 2. 删除成绩记录\n 3. 按学号查询成绩\n");
        printf(" 4. 按课程查询成绩\n 5. 按分数排序\n ");
        printf("6. 计算学生平均分\n 7. 计算课程平均分\n 0. 返回主菜单\n");
        printf("==================================\n请输入选项: ");
        if (scanf("%d",&choice) != 1) {
            while (getchar() != '\n') {}//清空输入缓冲区
            continue;
        }
        switch (choice) {
            case 1: {
                Score sc;
                memset(&sc,0,sizeof(sc));//初始化成绩结构体
                printf("学号: ");
                scanf("%s",sc.studentId);
                printf("课程号: ");
                scanf("%s",sc.courseId);
                printf("分数: ");
                scanf("%1f",&sc.value);
                printf("考试类型: ");
                scanf("%s",sc.examType);
                score_add(s,sc);
                break;
            }
            case 2: {
                char studentId[MAX_ID_LEN], courseId[MAX_ID_LEN];
                printf("学号: ");
                scanf("%s", studentId);
                printf("课程号: ");
                scanf("%s", courseId);
                score_remove(s,studentId,courseId);
                break;
            }
            case 3: {
                char studentId[MAX_ID_LEN];
                printf("学号: ");
                scanf("%s", studentId);
                score_findByStudent(s,studentId);
                break;
            }
            case 4: {
                char courseId[MAX_ID_LEN];
                printf("课程号: ");
                scanf("%s", courseId);
                score_findByCourse(s,courseId);
                break;
            }
            case 5: {
                int k;
                printf("请输入要显示的成绩条数: ");
                scanf("%d", &k);
                score_sortByScoreDesc(s, k);
                break;
            }
            case 6: {
                char studentId[MAX_ID_LEN];
                printf("学号: ");
                scanf("%s", studentId);
                score_avgByStudent(s,studentId);
                break;
            }
            case 7: {
                char courseId[MAX_ID_LEN];
                printf("课程号: ");
                scanf("%s", courseId);
                score_avgByCourse(s, courseId);
                break;
            }
            case 0: {
                return;
            }
            default:
                printf("无效选项，请重新输入\n");
        }
    }
}
