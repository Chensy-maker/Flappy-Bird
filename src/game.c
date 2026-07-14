#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <SDL2/SDL.h>
#include "game.h"
#include "bird.h"
#include "pipe.h"

GameScene g_scene;
Bird      g_bird;
PipeList  g_pipes;
int       g_score;
int       g_highscore;

int g_input_jump = 0;   
int pipe_timer = 0;     

void load_highscore(void)
{
    FILE *fp = fopen("data/highscore.txt", "r");
    g_highscore = 0;
    if (fp != NULL)
    {
        fscanf(fp, "%d", &g_highscore);
        fclose(fp);
    }
}

void save_highscore(void)
{
    
    FILE *fp = fopen("data/highscore.txt", "w");
    if (fp != NULL)
    {
        fprintf(fp, "%d", g_highscore);
        fclose(fp);
    }
}


void game_free_pipes(void)
{
    Pipe *p = g_pipes, *q;
    while (p != NULL)
    {
        q = p->next;
        free(p);
        p = q;
    }
    g_pipes = NULL;
}

void game_init(void)
{
    g_scene = SCENE_START;
    g_score = 0;
    g_pipes = NULL;
    pipe_timer = 0;
    g_input_jump = 0;

    // 重置小鸟位置速度
    g_bird.y = SCREEN_H / 2.0f;
    g_bird.vy = 0.0f;
    g_bird.frame = 0;

    load_highscore();
}

void game_switch_scene(GameScene s)
{
    g_scene = s;

    // 切换到非游戏状态统一重置
    if (s == SCENE_START || s == SCENE_READY || s == SCENE_OVER)
    {
        g_score = 0;
        pipe_timer = 0;
        game_free_pipes();

        g_bird.y = SCREEN_H / 2.0f;
        g_bird.vy = 0.0f;
        g_bird.frame = 0;
    }
}

void game_update(void)
{
    // 每帧清空按键标记
    g_input_jump = 0;

    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            SDL_Quit();
            exit(0);
        }

        // 空格跳跃
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE && !e.key.repeat)
        {
            g_input_jump = 1;
        }

        // 鼠标左键跳跃
        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
        {
            g_input_jump = 1;
        }
    }

    // 场景逻辑
    switch (g_scene)
    {
        case SCENE_START:
        {
            if (g_input_jump)
            {
                game_switch_scene(SCENE_READY);
            }
            break;
        }

        case SCENE_READY:
        {
            
            static float fly_dir = 0.05f;
            g_bird.y += fly_dir;

            if (g_bird.y > SCREEN_H / 2.0f + 3.0f) fly_dir = -0.05f;
            if (g_bird.y < SCREEN_H / 2.0f - 3.0f) fly_dir = 0.05f;

            g_bird.vy = 0.0f;

            if (g_input_jump)
            {
                game_switch_scene(SCENE_GAME);
            }
            break;
        }

        case SCENE_GAME:
        {
            // 跳跃响应
            if (g_input_jump)
            {
                bird_jump(&g_bird);
            }

            // 物理更新
            bird_apply_gravity(&g_bird);

            // 管道逻辑
            move_pipes();
            pipe_timer++;
            if (pipe_timer >= PIPE_SPAWN_INTERVAL)
            {
                spawn_pipe();
                pipe_timer = 0;
            }

            // 计分
            if (check_score())
            {
                g_score++;
            }

            // 碰撞检测 & 游戏结束
            if (check_collision())
            {
                if (g_score > g_highscore)
                {
                    g_highscore = g_score;
                    save_highscore();
                }
                game_switch_scene(SCENE_OVER);
            }

            break;
        }

        case SCENE_OVER:
        {
            if (g_input_jump)
            {
                game_switch_scene(SCENE_READY);
            }
            break;
        }
    }
}
