// ========== 全部必须头文件（解决 FILE/NULL/SDL/结构体未定义 ==========
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <SDL2/SDL.h>
#include "game.h"
#include "bird.h"
#include "pipe.h"
#include "config.h"

// ========== 全局变量外部声明（解决 g_scene/g_bird/g_score 等未定义 ==========
extern GameScene g_scene;
extern int g_score;
extern Pipe* g_pipes;
extern Bird g_bird;
extern int g_highscore;
extern int pipe_timer;

// ========== 文件读写函数前置（消除 conflicting types 警告 ==========
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

void game_free_pipes(void)
{
    Pipe *p = g_pipes, *q;
    while(p != NULL)
    {
        q = p->next;
        free(p);
        p = q;
    }
}

// ========== 游戏初始化 ==========
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

// ========== 切换场景 ==========
void game_switch_scene(GameScene s)
{
    g_scene = s;
    
    if (s == SCENE_READY || s == SCENE_OVER || s == SCENE_START)
    {
        g_score = 0;
        pipe_timer = 0;
        // 统一释放管道链表
        game_free_pipes();
        g_pipes = NULL;
        // 重置小鸟
        g_bird.y = SCREEN_H / 2.0f;
        g_bird.vy = 0.0f;
    }
}

// ========== 游戏更新主逻辑 ==========
void game_update(void)
{
    // 输入事件处理：空格 + 鼠标左键
    int key = 0;
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            SDL_Quit();
            exit(0);
        }
        // 空格按下
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE)
        {
            key = 1;
        }
        // 鼠标左键点击
        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
        {
            key = 1;
        }
    }

    switch (g_scene)
    {
        case SCENE_START:
            if (key)
            {
                game_switch_scene(SCENE_READY);
            }
            break;

        case SCENE_READY:
            g_bird.vy = 0;
            g_bird.y += 0.05f;
            if(g_bird.y > SCREEN_H/2 + 1) 
                g_bird.y = SCREEN_H/2 - 1;

            if(key)
            {
                game_switch_scene(SCENE_GAME);
            }
            break;

        case SCENE_GAME:
            if(key)
            {
                bird_jump(&g_bird);
            }

            bird_apply_gravity(&g_bird);
            move_pipes();

            pipe_timer++;
            if(pipe_timer >= PIPE_SPAWN_INTERVAL)
            {
                spawn_pipe();
                pipe_timer = 0;
            }

            // 计分
            if(check_score())
            {
                g_score++;
            }

            // 碰撞检测，加大括号截断场景穿透bug
            if(check_collision())
            {
                // 更新最高分并保存
                if(g_score > g_highscore)
                {
                    g_highscore = g_score;
                    save_highscore();
                }
                game_switch_scene(SCENE_OVER);
            }
            break;

        case SCENE_OVER:
            if(key)
            {
                game_switch_scene(SCENE_READY);
            }
            break;
    }
}