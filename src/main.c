#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "game.h"
#include "render.h"
#include "input.h"
#include "audio.h"

//目标单帧间隔毫秒数（FPS=60）
#define TARGET_DELTA (1000.0f / FPS)
//int main(int argc, char *argv[])增加兼容性，后续可以增加启动命令
//argc用于统计命令行参数个数，argv用于存储命令行参数字符串数组
int main(int argc, char *argv[])
{
    (void)argc; (void)argv;//避免编译器警告未使用参数

    srand((unsigned int)time(NULL));//以当前时间为种子生成随机数

    //初始化渲染模块：创建SDL窗口和渲染器，加载贴图资源
    if (render_init() != 0) {
        fprintf(stderr, "Failed to initialize render system.\n");
        return 1;
    }
    audio_init();//初始化音频，加载所有ogg音效文件
    game_init();//初始化游戏状态，设置初始场景、分数、小鸟位置等

    //Uint32为无符号32位整型，SDL_GetTicks()返回自SDL初始化以来的毫秒数
    //last_time记录上一帧的时间点，accumulator累积时间差，用于固定时间步长更新游戏逻辑
    Uint32 last_time = SDL_GetTicks();
    float accumulator = 0.0f;
    g_running = 1;//游戏主循环运行标志，1表示游戏正在运行，0表示退出游戏

    while (g_running && !input_quit_requested()) {
        Uint32 now = SDL_GetTicks();
        float delta = (float)(now - last_time);//本帧耗时毫秒数
        last_time = now;//更新本帧时刻
        
        //容错保护，如果程序卡顿、卡死，单帧间隔超过 100ms，强行把间隔限制为一帧标准时长
        if (delta > 100.0f) delta = TARGET_DELTA;
        accumulator += delta;

        while (accumulator >= TARGET_DELTA) {
            input_process_events();//捕获键盘、鼠标、窗口所有事件
            game_update();//更新游戏状态，包括小鸟位置、管道生成、碰撞检测、分数更新等
            accumulator -= TARGET_DELTA;//更新累加器
        }

        render_draw();//画面渲染
        
        //frame_time记录本帧渲染耗时，如果小于目标间隔，则延迟剩余时间，保证游戏帧率稳定
        //将画面帧率锁定在60FPS，避免游戏运行过快或过慢
        Uint32 frame_time = SDL_GetTicks() - now;
        if (frame_time < (Uint32)TARGET_DELTA) {
            SDL_Delay((Uint32)(TARGET_DELTA - frame_time));
        }
    }
    //循环结束后逆序释放资源
    //逆序释放，防止后创建的资源依赖于先创建的资源，避免访问已释放的资源导致崩溃
    audio_quit();//关闭音频设备
    game_free_pipes();//释放管道链表内存
    render_quit();//释放贴图资源，销毁渲染器和窗口

    printf("Thanks for playing! Final Score: %d\n", g_score);
    return 0;
}
