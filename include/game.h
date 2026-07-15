#ifndef GAME_H//防止这个头文件被多个地方
#define GAME_H//如果上一行判断成立（没定义过），立刻定义

#include "types.h"
#include <SDL2/SDL.h>

extern GameScene g_scene;//当前游戏场景
extern Bird      g_bird;//当前小鸟的状态
extern PipeList  g_pipes;//当前所有管道的链表头指针
extern int       g_score;//当前游戏分数
extern int       g_highscore;//历史最高分
extern int       g_countdown;//倒计时计数器，游戏开始前的倒计时
extern int       g_running;//游戏主循环是否继续运行的标记，0
extern float     g_bg_offset;//当前背景图的水平偏移量
extern float     g_ground_offset;//当前地面图的水平偏移量

void game_init(void);//初始化游戏状态
void game_update(void);//更新游戏状态
void game_switch_scene(GameScene s);//切换游戏场景
void game_free_pipes(void);//释放所有管道的内存
void game_cleanup_offscreen_pipes(void);//清理屏幕外的管道，释放内存

void load_highscore(void);//加载历史最高分
void save_highscore(void);//保存历史最高分

extern SDL_Renderer *g_renderer;//全局渲染器指针，供其他模块使用

#endif
