#ifndef RENDER_H
#define RENDER_H
#include "types.h"

// 初始化ncurses终端图形库、双缓冲
void render_init(void);
// 每一帧刷新画面
void render_draw(void);
// 结束渲染关闭ncurses
void render_quit(void);
// 绘制滚动背景、地面
void draw_bg(void);
void draw_ground(void);
// 绘制小鸟动画
void draw_bird(void);
// 绘制成对管道
void draw_pipes(void);
// 绘制分数、标题、结束提示文字
void draw_ui(void);
#endif
