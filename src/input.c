#include "input.h"

static bool space_down = false, space_prev = false;//空格键按下状态和上一次的按下状态
static bool click_down = false, click_prev = false;//鼠标左键按下状态和上一次的按下状态
static bool left_down = false, left_prev = false;//左方向键按下状态和上一次的按下状态
static bool right_down = false, right_prev = false;//右方向键按下状态和上一次的按下状态
static bool pause_down = false, pause_prev = false;//暂停键按下状态和上一次的按下状态
static bool quit_req = false;//是否请求退出游戏（点击窗口关闭按钮或按下ESC键）
int click_x = 0, click_y = 0;//鼠标点击的x坐标和y坐标

void input_process_events(void)//处理SDL事件队列，更新按键状态
{
    space_prev = space_down;//空格键上一次的按下状态
    click_prev = click_down;//鼠标左键上一次的按下状态
    left_prev = left_down;//左方向键上一次的按下状态
    right_prev = right_down;//右方向键上一次的按下状态
    pause_prev = pause_down;//暂停键上一次的按下状态
    SDL_Event e;//SDL事件结构体，用于存储从事件队列中获取的事件

    while (SDL_PollEvent(&e)) {
        switch (e.type) {//根据事件类型进行处理
        case SDL_QUIT://点击窗口关闭按钮
            quit_req = true; break;//请求退出游戏
        case SDL_KEYDOWN://按键按下事件
            switch (e.key.keysym.sym) {//根据按键符号进行处理
            case SDLK_SPACE:  space_down = true; break;//空格键按下
            case SDLK_p://P键按下
            case SDLK_PAUSE:  pause_down = true; break;//暂停键按下
            case SDLK_LEFT:   left_down = true; break;//左方向键按下
            case SDLK_RIGHT:  right_down = true; break;//右方向键按下
            case SDLK_ESCAPE: quit_req = true; break;//ESC键按下
            default: break;//其他按键不处理
            }
            break;
        case SDL_KEYUP:
            switch (e.key.keysym.sym) {//按键释放事件
            case SDLK_SPACE:  space_down = false; break;//空格键释放
            case SDLK_p://P键释放
            case SDLK_PAUSE:  pause_down = false; break;//暂停键释放
            case SDLK_LEFT:   left_down = false; break;//左方向键释放
            case SDLK_RIGHT:  right_down = false; break;//右方向键释放
            default: break;//其他按键不处理
            }
            break;
        case SDL_MOUSEBUTTONDOWN://鼠标按下事件
            if (e.button.button == SDL_BUTTON_LEFT) {//鼠标左键按下
                click_down = true;//鼠标左键按下状态为true
                click_x = e.button.x;//鼠标点击的x坐标
                click_y = e.button.y;//鼠标点击的y坐标
            }
            break;
        case SDL_MOUSEBUTTONUP://鼠标释放事件
            if (e.button.button == SDL_BUTTON_LEFT) {//鼠标左键释放
                click_down = false;//鼠标左键按下状态为false
            }
            break;
        }
    }
}

bool input_space_pressed(void)    { return space_down; }//空格键是否按下
bool input_space_triggered(void)  { return space_down && !space_prev; }//空格键是否刚刚按下（从未按下到按下的瞬间）
bool input_click_triggered(void)  { return click_down && !click_prev; }//鼠标左键是否刚刚点击（从未按下到按下的瞬间）
bool input_left_triggered(void)   { return left_down && !left_prev; }//左方向键是否刚刚按下（从未按下到按下的瞬间）
bool input_right_triggered(void)  { return right_down && !right_prev; }//右方向键是否刚刚按下（从未按下到按下的瞬间）
bool input_pause_triggered(void)  { return pause_down && !pause_prev; }//暂停键是否刚刚按下（从未按下到按下的瞬间）
bool input_quit_requested(void)   { return quit_req; }//是否请求退出游戏