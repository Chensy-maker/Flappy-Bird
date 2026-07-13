#include "physics.h"
#include "game.h"
#include <stdlib.h>

void bird_apply_gravity(Bird *b)
{
    b->vy += GRAVITY;
    b->y += b->vy;
}

void bird_jump(Bird *b)
{
    b->vy = JUMP_FORCE;
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
    //碰撞地面
    if (g_bird.y + BIRD_SIZE >= SCREEN_H - 2)
        return 1;

    //碰撞顶部
    if (g_bird.y <= 0)
        return 1;

    
    Pipe *p = g_pipes;
    Rect bird_rect = {
        BIRD_X, (int)g_bird.y,
        BIRD_SIZE, BIRD_SIZE
    };

    while (p)
    {
       //上管道
        Rect up_pipe = {p->x, 0, PIPE_WIDTH, p->gap_y};
        //下管道
        Rect down_pipe = {p->x, p->gap_y + PIPE_GAP,
                          PIPE_WIDTH, SCREEN_H};

        if (rect_collide(bird_rect, up_pipe) ||
            rect_collide(bird_rect, down_pipe))
        {
            return 1;
        }
        p = p->next;
    }
    return 0;
}

void spawn_pipe(void)
{
    Pipe *new_p = (Pipe*)malloc(sizeof(Pipe));
    new_p->x = SCREEN_W;
    new_p->gap_y = rand() % (SCREEN_H - PIPE_GAP - 8) + 4;
    new_p->next = g_pipes;
    g_pipes = new_p;
}

void move_pipes(void)
{
    Pipe *p = g_pipes;
    while (p)
    {
        p->x -= PIPE_SPEED;
        p = p->next;
    }
}

int check_score(void)
{
    Pipe *p = g_pipes;
    while (p)
    {
        
        if (p->x + PIPE_WIDTH == BIRD_X)
        {
            return 1;
        }
        p = p->next;
    }
    return 0;
}