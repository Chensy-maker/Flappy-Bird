#ifndef GAME_H
#define GAME_H

#include "types.h"

// ============ 全局变量 ============
extern GameScene g_scene;
extern Bird      g_bird;
extern PipeList  g_pipes;
extern int       g_score;
extern int       g_highscore;

// ============ 游戏核心函数 ============
void game_init(void);
void game_update(void);
void game_switch_scene(GameScene s);
void game_free_pipes(void);

// ============ 数据持久化 ============
void load_highscore(void);
void save_highscore(void);

#endif
