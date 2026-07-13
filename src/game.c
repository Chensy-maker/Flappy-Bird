#include "game.h"
#include "physics.h"
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

    // 初始化小鸟
    g_bird.y  = SCREEN_H / 2.0f;
    g_bird.vy = 0.0f;
    g_bird.frame = 0;

    load_highscore();
}

void game_switch_scene(GameScene s)
{
    g_scene = s;
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
            
            g_bird.vy = 0;
            g_bird.y += 0.05f;
            if(g_bird.y > SCREEN_H/2 + 1) g_bird.y = SCREEN_H/2 - 1;

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

            if(check_collision())
            {
                game_switch_scene(SCENE_OVER);
                
                if(g_score > g_highscore)
                {
                    g_highscore = g_score;
                    save_highscore();
                }
            }
            break;

        case SCENE_OVER:
            
            if(key)
            {
                
                game_free_pipes();
                g_pipes = NULL;
                g_score = 0;
                pipe_timer = 0;
                g_bird.y  = SCREEN_H / 2.0f;
                g_bird.vy = 0.0f;
                game_switch_scene(SCENE_READY);
            }
            break;
    }
}

void game_free_pipes(void)
{
    Pipe *p = g_pipes, *q;
    while(p)
    {
        q = p->next;
        free(p);
        p = q;
    }
}

void load_highscore(void)
{
    FILE *fp = fopen("data/highscore.txt", "r");
    g_highscore = 0;
    if(fp)
    {
        fscanf(fp, "%d", &g_highscore);
        fclose(fp);
    }
}

void save_highscore(void)
{
    FILE *fp = fopen("data/highscore.txt", "w");
    if(fp)
    {
        fprintf(fp, "%d", g_highscore);
        fclose(fp);
    }
}