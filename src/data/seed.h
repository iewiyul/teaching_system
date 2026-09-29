/*
种子数据初始化：5 课程 / 15 教师 / 350 学生 / 15 任课边 / 选课边 / rebuildIndexes
负责人：陆奕炜
*/
#ifndef SEED_H
#define SEED_H

/* 加载种子数据（README §6.5）。依赖 main.c 先按顺序 create 各 Service，
 * 且 g_operationHistory 必须非 NULL（add/remove/update 会调 history_record）。 */
void init_seed_data(void);

#endif /* SEED_H */