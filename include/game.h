#ifndef GAME_H
#define GAME_H

#include "types.h"
#include <SDL2/SDL.h>

extern GameScene g_scene;//当前游戏场景
extern Bird      g_bird;//小鸟结构体实例
extern PipeList  g_pipes;//管道链表头指针
extern int       g_score;//当前得分
extern int       g_highscore;//历史最高分
extern int       g_countdown;//准备场景倒计时
extern int       g_running;//游戏主循环运行标志
extern int       g_paused;//游戏暂停标志
extern int       g_bird_color;//小鸟颜色索引，0表示红色，1表示蓝色，2表示黄色
extern int       g_is_night;//是否为夜间模式，1表示夜间，0表示白天
extern float     g_bg_offset;//背景滚动偏移量
extern float     g_ground_offset;//地面滚动偏移量

void game_init(void);//初始化游戏状态
void game_update(void);//更新游戏状态
void game_switch_scene(GameScene s);//切换游戏场景
void game_free_pipes(void);//释放管道链表内存
void game_cleanup_offscreen_pipes(void);//清理已经完全移出屏幕左侧的管道，释放它们的内存

void load_highscore(void);//加载历史最高分
void save_highscore(void);//保存历史最高分

extern SDL_Renderer *g_renderer;//SDL渲染器实例

#endif
