#ifndef INPUT_H//防止这个头文件被多个地方 #include 时重复包含，报重定义编译错误
#define INPUT_H//如果上一行判断成立（没定义过），立刻定义 INPUT_H 宏

#include <SDL2/SDL.h>//引入 SDL2 图形库的主头文件
#include <stdbool.h>//C99 标准头文件，提供布尔类型

void input_process_events(void);//处理所有 SDL 输入事件的主函数
bool input_space_pressed(void);//查询空格键当前是否持续按住
bool input_space_triggered(void);//查询空格键是否刚刚按下
bool input_quit_requested(void);//查询是否收到退出游戏的请求

#endif
