#include "physics.h"
#include "game.h"
#include <stdlib.h>

void bird_apply_gravity(Bird *b)
{
    b->vy += GRAVITY;
    if (b->vy > MAX_FALL_SPEED)
        b->vy = MAX_FALL_SPEED;
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
    if (g_bird.y + BIRD_H >= SCREEN_H - GROUND_HEIGHT)
        return 1;
    if (g_bird.y <= 0)
        return 1;

    Pipe *p = g_pipes;
    Rect bird_rect = {
        BIRD_X + 4, g_bird.y + 2,
        BIRD_W - 8, BIRD_H - 4
    };

    while (p) {
        Rect up_pipe = {p->x, 0, PIPE_WIDTH, p->gap_center - PIPE_GAP / 2};
        Rect down_pipe = {p->x, p->gap_center + PIPE_GAP / 2,
                          PIPE_WIDTH, SCREEN_H};

        if (rect_collide(bird_rect, up_pipe) ||
            rect_collide(bird_rect, down_pipe)) {
            return 1;
        }
        p = p->next;
    }
    return 0;
}

void spawn_pipe(void)
{
    Pipe *new_p = (Pipe*)malloc(sizeof(Pipe));
    if (!new_p) return;

    new_p->x = SCREEN_W;
    int min_center = PIPE_MIN_HEIGHT + PIPE_GAP / 2;
    int max_center = (SCREEN_H - GROUND_HEIGHT) - PIPE_MIN_HEIGHT - PIPE_GAP / 2;
    if (max_center <= min_center) max_center = min_center + 50;
    new_p->gap_center = rand() % (max_center - min_center + 1) + min_center;
    new_p->scored = 0;
    new_p->next = g_pipes;
    g_pipes = new_p;
}

void move_pipes(void)
{
    Pipe *p = g_pipes;
    while (p) {
        p->x -= PIPE_SPEED;
        p = p->next;
    }
}

int check_score(void)
{
    Pipe *p = g_pipes;
    while (p) {
        if (!p->scored && p->x + PIPE_WIDTH < BIRD_X) {
            p->scored = 1;
            return 1;
        }
        p = p->next;
    }
    return 0;
}
