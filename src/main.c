#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "game.h"
#include "render.h"
#include "input.h"

// 帧率控制：60 FPS = 16667 微秒/帧
#define FRAME_TIME_US 16667

int main()
{
    // 随机数种子（用于生成随机管道高度）
    srand(time(NULL));
    
    // 初始化游戏系统
    render_init();
    game_init();
    
    int running = 1;
    int frame_count = 0;
    
    // 主游戏循环
    while (running)
    {
        // 1. 更新游戏逻辑
        game_update();
        
        // 2. 绘制整屏画面（双缓冲：内部只调用一次refresh）
        render_draw();
        
        // 3. 小鸟翅膀动画：每5帧切换一次
        frame_count++;
        if (frame_count % 5 == 0) {
            g_bird.frame = 1 - g_bird.frame;
        }
        
        // 4. 帧率控制：固定60 FPS，避免CPU占用过高
        usleep(FRAME_TIME_US);
        
        // 5. 检查退出键（Q键退出）
        int key = input_get_key();
        if (key == 'q' || key == 'Q') {
            running = 0;
        }
    }
    
    // 清理资源
    render_quit();
    game_free_pipes();
    
    printf("Thanks for playing! Final Score: %d\n", g_score);
    
    return 0;
}
