#include "input.h"
#include <SDL2/SDL.h>

void input_init(void)
{
// SDL事件系统自动初始化，无需额外操作
}

int input_get_key(void)
{
SDL_Event event;
while (SDL_PollEvent(&event)) {
if (event.type == SDL_QUIT) {
return 'q';  // 窗口关闭按钮
}
if (event.type == SDL_KEYDOWN) {
switch (event.key.keysym.sym) {
case SDLK_SPACE:
case SDLK_UP:
return ' ';  // 跳跃键
case SDLK_q:
case SDLK_ESCAPE:
return 'q';  // 退出键
default:
return event.key.keysym.sym;
}
}
if (event.type == SDL_MOUSEBUTTONDOWN) {
return ' ';  // 鼠标点击也算跳跃
}
}
return 0;
}

void input_restore(void)
{
// SDL事件系统自动清理，无需额外操作
}