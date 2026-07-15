#include "input.h"

static bool space_down = false;//当前帧空格键是否处于按下状态
static bool space_prev = false;//上一帧空格键的按下状态，用来判断 “刚按下瞬间”
static bool quit_req = false;//标记是否需要退出游戏，true= 收到关闭信号

void input_process_events(void)
{
    SDL_Event e;
    space_prev = space_down;//在处理本帧新事件前，把上一帧的按键状态存入 space_prev，保存历史状态，用于区分长按和单次点击

    while (SDL_PollEvent(&e)) {//从 SDL 事件队列取出一条事件存入e
        switch (e.type) {//判断当前取出事件的类型
        case SDL_QUIT:
            quit_req = true;//把退出标记 quit_req 置为 true
            break;
        case SDL_KEYDOWN:
            if (e.key.keysym.sym == SDLK_SPACE)//判断按键是空格 
                space_down = true;
            if (e.key.keysym.sym == SDLK_ESCAPE)//判断按键是 ESC 
                quit_req = true;
            break;
        case SDL_KEYUP:
            if (e.key.keysym.sym == SDLK_SPACE)//判断按键是空格
                space_down = false;//标记空格当前处于按住状态
            break;
        }
    }
}

bool input_space_pressed(void)//查询空格键当前是否持续按住
{
    return space_down;
}

bool input_space_triggered(void)//查询空格键是否刚刚按下
{
    return space_down && !space_prev;
}

bool input_quit_requested(void)//查询是否收到退出游戏的请求
{
    return quit_req;
}
