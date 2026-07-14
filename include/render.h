#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>

#define SCREEN_WIDTH  288
#define SCREEN_HEIGHT 512


extern SDL_Renderer *g_renderer;  // 全局渲染器指针，供其他模块使用 
extern SDL_Window *g_window;

// 渲染模块接口
int  render_init(void);    // 初始化SDL和加载资源
void render_quit(void);    // 清理SDL资源
void render_draw(void);    // 绘制当前帧
#endif
