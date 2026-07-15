#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include <stdbool.h>

void input_process_events(void);//处理SDL事件队列，更新按键状态
bool input_space_pressed(void);//空格键是否按下
bool input_space_triggered(void);//空格键是否刚刚按下（从未按下到按下的瞬间）
bool input_click_triggered(void);//鼠标左键是否刚刚点击（从未按下到按下的瞬间）
bool input_left_triggered(void);//左方向键是否刚刚按下（从未按下到按下的瞬间）
bool input_right_triggered(void);//右方向键是否刚刚按下（从未按下到按下的瞬间）
bool input_pause_triggered(void);//暂停键（P键或Pause键）是否刚刚按下（从未按下到按下的瞬间）
bool input_quit_requested(void);//是否请求退出游戏（点击窗口关闭按钮或按下ESC键）

extern int click_x;//鼠标点击的x坐标
extern int click_y;//鼠标点击的y坐标

#endif
