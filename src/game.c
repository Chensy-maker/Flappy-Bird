#include "game.h"
#include "physics.h"
#include "input.h" 
#include <stdio.h>
#include <stdlib.h>

GameScene g_scene;
Bird      g_bird;
PipeList  g_pipes;
int       g_score;
int       g_highscore;

static int pipe_timer = 0;

void game_init(void)
{
    g_scene = SCENE_START;
    g_score = 0;
    g_pipes = NULL;
    pipe_timer = 0; // 初始化管道计时器

    // 初始化小鸟
    g_bird.y  = SCREEN_H / 2.0f;
    g_bird.vy = 0.0f;
    g_bird.frame = 0;

    load_highscore();
}

void game_switch_scene(GameScene s)
{
    g_scene = s;
    
    if (s == SCENE_READY || s == SCENE_OVER)
    {
        g_score = 0;
        pipe_timer = 0;
        // 清空所有管道
        PipeList p = g_pipes, q;
        while (p != NULL)
        {
            q = p->next;
            free(p);
            p = q;
        }
        g_pipes = NULL;
        // 重置小鸟位置速度
        g_bird.y = SCREEN_H / 2.0f;
        g_bird.vy = 0.0f;
    }
}

void game_update(void)
{
    int key = input_get_key();

    switch(g_scene)
    {
        case SCENE_START:
            if(key)
            {
                game_switch_scene(SCENE_READY);
            }
            break;
        case SCENE_READY:
            // 游戏运行逻辑省略，保留原有分支结构
            break;
        case SCENE_OVER:
            // 游戏结束更新最高分
            if (g_score > g_highscore)
            {
                g_highscore = g_score;
                save_highscore();
            }
            if(key)
            {
                game_switch_scene(SCENE_START);
            }
            break;
        default:
            break;
    }
}

void load_highscore(void)
{
    FILE *fp = fopen("data/highscore.txt", "r");
    g_highscore = 0;
    if(fp != NULL)
    {
        fscanf(fp, "%d", &g_highscore);
        fclose(fp);
    }
}

void save_highscore(void)
{
    FILE *fp = fopen("data/highscore.txt", "w");
    if(fp != NULL)
    {
        fprintf(fp, "%d", g_highscore);
        fclose(fp);
    }
    
}