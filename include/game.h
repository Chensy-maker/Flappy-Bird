#ifndef GAME_H
#define GAME_H
#include "types.h"

extern GameScene g_scene;
extern Bird g_bird;
extern PipeList g_pipes;
extern int g_score;
extern int g_highscore;

// 初始化游戏所有数据
void game_init(void);
// 每一帧更新游戏逻辑
void game_update(void);
// 切换场景
void game_switch_scene(GameScene s);
// 释放管道链表内存
void game_free_pipes(void);
// 读写最高分文件
void load_highscore(void);
void save_highscore(void);
#endif
