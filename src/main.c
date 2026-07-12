#include <stdio.h>
#include "game.h"
#include "render.h"
#include "input.h"

int main()
{
    input_init();
    render_init();
    game_init();

    while(1)
    {
        // 1. 读取按键
        int key = input_get_key();
        // 2. 游戏逻辑更新（场景、物理、管道、碰撞）
        game_update();
        // 3. 绘制整屏画面
        render_draw();
    }

    render_quit();
    input_restore();
    game_free_pipes();
    return 0;
}
