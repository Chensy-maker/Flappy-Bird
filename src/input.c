#include "input.h"

static bool space_down = false;//当前帧空格键是否处于按下状态
static bool space_prev = false;//上一帧空格键的按下状态，用来判断 “刚按下瞬间”
static bool quit_req = false;//标记是否需要退出游戏，true= 收到关闭信号

void input_process_events(void)
{
    SDL_Event e;
    space_prev = space_down;//在处理本帧新事件前，把上一帧的按键状态存入 space_prev，保存历史状态，用于区分长按和单次点击
static bool space_down = false, space_prev = false;//当前帧空格键是否处于按下状态，上一帧空格键的按下状态，用来判断 “刚按下瞬间”
static bool click_down = false, click_prev = false;//当前帧鼠标左键是否处于按下状态，上一帧鼠标左键的按下状态，用来判断 “刚按下瞬间”
static bool left_down = false, left_prev = false;//当前帧左方向键是否处于按下状态，上一帧左方向键的按下状态，用来判断 “刚按下瞬间”
static bool right_down = false, right_prev = false;//当前帧右方向键是否处于按下状态，上一帧右方向键的按下状态，用来判断 “刚按下瞬间”
static bool pause_down = false, pause_prev = false;//当前帧暂停键是否处于按下状态，上一帧暂停键的按下状态，用来判断 “刚按下瞬间”
static bool quit_req = false;//标记是否需要退出游戏，true= 收到关闭信号
int click_x = 0, click_y = 0;

void input_process_events(void)
{
    space_prev = space_down;
    click_prev = click_down;
    left_prev = left_down;
    right_prev = right_down;
    pause_prev = pause_down;
    SDL_Event e;

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
            quit_req = true; break;
        case SDL_KEYDOWN:
            switch (e.key.keysym.sym) {
            case SDLK_SPACE:  space_down = true; break;//标记空格当前处于按住状态
            case SDLK_p://标记暂停键当前处于按住状态
            case SDLK_PAUSE:  pause_down = true; break;//标记暂停键当前处于按住状态
            case SDLK_LEFT:   left_down = true; break;
            case SDLK_RIGHT:  right_down = true; break;
            case SDLK_ESCAPE: quit_req = true; break;
            default: break;
            }
            break;
        case SDL_KEYUP:
            switch (e.key.keysym.sym) {
            case SDLK_SPACE:  space_down = false; break;
            case SDLK_p:
            case SDLK_PAUSE:  pause_down = false; break;
            case SDLK_LEFT:   left_down = false; break;
            case SDLK_RIGHT:  right_down = false; break;
            default: break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (e.button.button == SDL_BUTTON_LEFT) {//如果鼠标左键按下
                click_down = true;
                click_x = e.button.x;
                click_y = e.button.y;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (e.button.button == SDL_BUTTON_LEFT) {//如果鼠标左键抬起
                click_down = false;
            }
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
bool input_space_pressed(void)    { return space_down; }//查询空格键当前是否持续按住
bool input_space_triggered(void)  { return space_down && !space_prev; }//查询空格键是否刚刚按下
bool input_click_triggered(void)  { return click_down && !click_prev; }//查询鼠标左键是否刚刚按下
bool input_left_triggered(void)   { return left_down && !left_prev; }//查询左方向键是否刚刚按下
bool input_right_triggered(void)  { return right_down && !right_prev; }//查询右方向键是否刚刚按下
bool input_pause_triggered(void)  { return pause_down && !pause_prev; }//查询暂停键（P 或 Pause）是否刚刚按下
bool input_quit_requested(void)   { return quit_req; }//查询是否收到退出游戏的请求
