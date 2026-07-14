#include "game.h"
#include "physics.h"
#include "input.h"
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

g_bird.y  = SCREEN_H / 2.0f;
g_bird.vy = 0.0f;
g_bird.frame = 0;

load_highscore();
}

void game_switch_scene(GameScene s)
{
g_scene = s;

if (s == SCENE_READY || s == SCENE_OVER) {
g_score = 0;
pipe_timer = 0;

// 清空所有管道
game_free_pipes();

// 重置小鸟
g_bird.y = SCREEN_H / 2.0f;
g_bird.vy = 0.0f;
}
}

void game_update(void)
{
int key = input_get_key();

switch (g_scene) {
case SCENE_START:
if (key == ' ' || key == '\r') {
game_switch_scene(SCENE_READY);
}
break;

case SCENE_READY:
if (key == ' ') {
game_switch_scene(SCENE_GAME);
}
break;

case SCENE_GAME:
// 跳跃
if (key == ' ') {
bird_jump(&g_bird);
}

// 重力和移动
bird_apply_gravity(&g_bird);
move_pipes();

// 生成管道
pipe_timer++;
if (pipe_timer >= PIPE_SPAWN_INTERVAL) {
spawn_pipe();
pipe_timer = 0;
}

// 检查得分
if (check_score()) {
g_score++;
}

// 检查碰撞
if (check_collision()) {
game_switch_scene(SCENE_OVER);
}
break;

case SCENE_OVER:
// 更新最高分
if (g_score > g_highscore) {
g_highscore = g_score;
save_highscore();
}

if (key == ' ' || key == '\r') {
game_switch_scene(SCENE_START);
}
break;
}
}

void game_free_pipes(void)
{
PipeList p = g_pipes, q;
while (p != NULL) {
q = p->next;
free(p);
p = q;
}
g_pipes = NULL;
}

void load_highscore(void)
{
FILE *fp = fopen("data/highscore.txt", "r");
g_highscore = 0;
if (fp != NULL) {
fscanf(fp, "%d", &g_highscore);
fclose(fp);
}
}

void save_highscore(void)
{
// 确保data目录存在
system("mkdir -p data");

FILE *fp = fopen("data/highscore.txt", "w");
if (fp != NULL) {
fprintf(fp, "%d", g_highscore);
fclose(fp);
}
}

