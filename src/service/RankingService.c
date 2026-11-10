/*
负责人：成员 E
*/
#define _CRT_SECURE_NO_WARNINGS
#include "RankingService.h"
#include "ds/Heap.h"
#include "StudentService.h"
#include "TeacherService.h"
#include "ds/LinkedList.h"
#include "ds/SeqList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BENCH_REPS 100 //性能测试重复次数

RankingService* ranking_service_create(StudentService* ss, TeacherService* ts) {
    RankingService* r = (RankingService*)malloc(sizeof(RankingService));
    if (!r)return NULL;
    r->studentSvc = ss;
    r->teacherSvc = ts;
    return r;
}
void  ranking_service_destroy(RankingService* r) { 
    if (!r)return;
    free(r);
}
//学生成绩排名
void   ranking_topKStudents(StudentService* ss, int k) { 
    if (!ss || !ss->students || k<=0 ) {
        printf("无数据或 K 值不合法\n");
        return;
    }
    int n = linkedlist_size(ss->students);// 获取学生总数
    if (n == 0) {
        printf("无学生数据\n");
        return;
    }
    if (k>n) {
        k = n; // 如果 k 大于学生总数，则调整 k 为学生总数
    }
    Student* all = (Student*)malloc(sizeof(Student)*n);// 创建一个数组来存储所有学生
    if (!all)return;
    int idx = 0;
    for (ListNode* p = ss->students->head;p;p = p->next) {
        all[idx++] = p->data;
    }
    Student* top = (Student*)malloc(sizeof(Student)*k);// 创建一个数组来存储前 k 名学生
    if (!top) {
        free(all);
        return;
    }
    clock_t t0 = clock();
    for (int r = 0;r<BENCH_REPS;r++) {// 重复 BENCH_REPS 次以进行性能测试
        heap_topK_max(all,n,k,top);
    }
    clock_t t1 = clock();
    double us = (double)(t1 - t0) * 1e6 / CLOCKS_PER_SEC / BENCH_REPS;
    printf("[堆 TopK 算法] 数据规模 %d,提取 Top %d\n",n,k);
    printf("--- 学生绩点 Top %d ---\n",k);
    printf("排名\t学号\t姓名\t绩点\n");
    for (int i = 0;i < k;i++) {
        printf("%d\t%s\t%s\t%.1f\n", i + 1, top[i].id, top[i].name, top[i].gpa);
    }
    printf(" TopK 耗时：%.2f us (%d取平均)\n",us,BENCH_REPS);
    free(all);
    free(top);
}
//教师评分排名
static void teacher_sift_down(Teacher* a, int i, int n) {
    while (1) {
        int l = 2 * i + 1;//左孩子下标
        int r = 2 * i + 2;//右孩子下标
        int best = i;//假设此时 i 最应该在上面
        if (l < n && a[l].rating < a[best].rating)//若左孩子存在，将左孩子与父亲作比较
        {
            best = l;
        }
        if (r < n && a[r].rating < a[best].rating)//若右孩子存在，将右孩子与父亲作比较
        {
            best = r;
        }
        if (best == i) {
            break;
        }
        Teacher t = a[i]; a[i] = a[best]; a[best] = t;//和更优先交换位置
        i = best;//继续下沉
    }
}
static void teacher_topK_internal(Teacher* all,int n,int k,Teacher* result) {
    int sz = 0;
    for (int i = 0; i < n;i++) {
        if (sz < k) {//堆未满
            result[sz++] = all[i];//未满直接入堆
            int j = sz - 1;
            while (j>0) {//上浮
                int p = (j - 1)/2;
                if (result[j].rating < result[p].rating) {//小顶堆
                    Teacher tmp = result[j]; result[j] = result[p]; result[p] = tmp;
                    j = p;
                }
                else break;

            }
        }
        else if (all[i].rating > result[0].rating) {//大于堆顶，替换堆顶
            result[0] = all[i];
            teacher_sift_down(result, 0, sz);//下沉

        }
    }
    for (int i = k - 1;i > 0;i--) {//降序排列
        Teacher tmp = result[0]; result[0] = result[i]; result[i] = tmp;
        teacher_sift_down(result, 0, i);//下沉
    }
}
void   ranking_topKTeachers(TeacherService* ts, int k) { 
    if (!ts || !ts->teachers || k <= 0) {
        printf("无数据或 K 值不合法\n");
        return;
    }
    int n = seqlist_size(ts->teachers);// 获取教师总数
    if (n == 0) {
        printf("无教师数据\n");
        return;
    }
    if (k > n) {
        k = n; // 如果 k 大于教师总数，则调整 k 为教师总数
    }
    Teacher* all = (Teacher*)malloc(sizeof(Teacher) * n);// 创建一个临时数组来存储所有教师
    if (!all)return;
    for (int i = 0;i < n;i++) {
        all[i++] = *seqlist_at(ts->teachers,i);
    }
   Teacher* top = (Teacher*)malloc(sizeof(Teacher) * k);// 创建一个数组来存储前 k 名学生
    if (!top) {
        free(all);
        return;
    }
    clock_t t0 = clock();
    for (int r = 0;r < BENCH_REPS;r++) {// 重复 BENCH_REPS 次以进行性能测试
        teacher_topK_internal(all, n, k, top);
    }
    clock_t t1 = clock();
    double us = (double)(t1 - t0) * 1e6 / CLOCKS_PER_SEC / BENCH_REPS;
    printf("[堆 TopK 算法] 数据规模 %d,提取 Top %d\n", n, k);
    printf("--- 教师授课评分 Top %d ---\n", k);
    printf("排名\t共号\t姓名\t评分\n");
    for (int i = 0;i < k;i++) {
        printf("%d\t%s\t%s\t%.1f\n", i + 1, top[i].id, top[i].name, top[i].rating);
    }
    printf(" TopK 耗时：%.2f us (%d取平均)\n", us, BENCH_REPS);
    free(all);
    free(top);
}
//排行榜菜单
void   ranking_service_menu(RankingService* r) {
    int choice;
    while (1) {
        printf("\n========== 排行榜 ==========\n");
        printf(" 1. 学生绩点 TopK\n");
        printf(" 2. 教师授课评分 TopK\n");
        printf(" 0. 返回主菜单\n");
        printf("==================================\n请输入选项: ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n') {}//清空输入缓冲区
            continue;
        }
        switch (choice) {
            case 1: {
                int k;
                printf("请输入 k 值:");
                scanf("%d",&k);
                ranking_topKStudents(r->studentSvc,k);
                break;
            }
            case 2: {
                int k;
                printf("请输入 k 值:");
                scanf("%d", &k);
                ranking_topKTeachers(r->teacherSvc, k);
                break;
            }
            case 0: {
                return;
            }
            default:
                printf("无效选项\n");
        }
    }
}
