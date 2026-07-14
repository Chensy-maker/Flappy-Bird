#include "physics.h"
#include "game.h"
#include <stdlib.h>

void bird_apply_gravity(Bird *b)
{
b->vy += GRAVITY;
if (b->vy > MAX_FALL_SPEED) {
b->vy = MAX_FALL_SPEED;
}
b->y += b->vy;
}

void bird_jump(Bird *b)
{
b->vy = BIRD_JUMP_VEL;
}

int rect_collide(Rect a, Rect b)
{
if (a.x + a.w <= b.x) return 0;
if (b.x + b.w <= a.x) return 0;
if (a.y + a.h <= b.y) return 0;
if (b.y + b.h <= a.y) return 0;
return 1;
}

int check_collision(void)
{
// 碰撞地面
if (g_bird.y + BIRD_H >= SCREEN_H - 112) {
return 1;
}
// 碰撞天花板
if (g_bird.y <= 0) {
return 1;
}

Rect bird_rect = {
BIRD_X, (int)g_bird.y,
BIRD_W, BIRD_H
};

struct Pipe *p = g_pipes;
while (p) {
// 上管道
Rect up_pipe = {(int)p->x, 0, PIPE_WIDTH, p->gap_y - PIPE_GAP/2};
// 下管道
Rect down_pipe = {(int)p->x, p->gap_y + PIPE_GAP/2, PIPE_WIDTH, SCREEN_H};

if (rect_collide(bird_rect, up_pipe) || rect_collide(bird_rect, down_pipe)) {
return 1;
}
p = p->next;
}
return 0;
}

void spawn_pipe(void)
{
struct Pipe *new_p = (struct Pipe*)malloc(sizeof(struct Pipe));

new_p->x = SCREEN_W;
// 随机生成管道缝隙中心位置
new_p->gap_y = PIPE_MIN_HEIGHT + rand() % (PIPE_MAX_HEIGHT - PIPE_MIN_HEIGHT);
new_p->scored = 0;
new_p->next = g_pipes;
g_pipes = new_p;
}

void move_pipes(void)
{
struct Pipe *p = g_pipes, *prev = NULL;
while (p) {
p->x -= PIPE_SPEED;
// 删除超出屏幕的管道
if (p->x + PIPE_WIDTH < 0) {
if (prev) {
prev->next = p->next;
} else {
g_pipes = p->next;
}
struct Pipe *temp = p;
p = p->next;
free(temp);
} else {
prev = p;
p = p->next;
}
}
}

int check_score(void)
{
struct Pipe *p = g_pipes;
while (p) {
if (!p->scored && p->x + PIPE_WIDTH < BIRD_X) {
p->scored = 1;
return 1;
}
p = p->next;
}
return 0;
}
