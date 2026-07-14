#include "game.h"
#include "physics.h"
#include "input.h"
#include "audio.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

GameScene g_scene;
Bird      g_bird;
PipeList  g_pipes;
int       g_score;
int       g_highscore;
int       g_countdown;
int       g_running;
float     g_bg_offset;
float     g_ground_offset;
SDL_Renderer *g_renderer = NULL;

static int pipe_timer = 0;
static int ready_timer = 0;
static int anim_counter = 0;
static float float_t = 0;

void game_init(void)
{
    g_scene = SCENE_START;
    g_score = 0;
    g_pipes = NULL;
    pipe_timer = 0;
    ready_timer = 0;
    anim_counter = 0;
    float_t = 0;
    g_bg_offset = 0.0f;
    g_ground_offset = 0.0f;

    g_bird.y = BIRD_BODY_Y;
    g_bird.vy = 0.0f;
    g_bird.frame = 1;

    load_highscore();
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

void game_cleanup_offscreen_pipes(void)
{
    PipeList *curr = &g_pipes;
    while (*curr != NULL) {
        if ((*curr)->x + PIPE_WIDTH < -50) {
            PipeList to_free = *curr;
            *curr = (*curr)->next;
            free(to_free);
        } else {
            curr = &(*curr)->next;
        }
    }
}

void game_switch_scene(GameScene s)
{
    g_scene = s;

    if (s == SCENE_READY) {
        g_score = 0;
        pipe_timer = 0;
        ready_timer = 0;
        g_bird.y = BIRD_BODY_Y;
        g_bird.vy = 0.0f;
        g_bird.frame = 1;
        game_free_pipes();
    }

    if (s == SCENE_START) {
        g_bird.y = BIRD_BODY_Y;
        g_bird.vy = 0.0f;
        g_bird.frame = 1;
        game_free_pipes();
        g_score = 0;
    }
}

void game_update(void)
{
    switch (g_scene) {
    case SCENE_START: {
        g_bg_offset -= BG_SCROLL_SPEED;
        g_ground_offset -= GROUND_SCROLL_SPEED;

        float_t += 0.05f;
        g_bird.y = BIRD_BODY_Y + (float)sin(float_t) * 5.0f;

        anim_counter++;
        if (anim_counter >= 8) {
            anim_counter = 0;
            g_bird.frame = (g_bird.frame + 1) % 3;
        }

        if (input_space_triggered()) {
            audio_play_swoosh();
            game_switch_scene(SCENE_READY);
        }
        break;
    }

    case SCENE_READY: {
        g_bg_offset -= BG_SCROLL_SPEED;
        g_ground_offset -= GROUND_SCROLL_SPEED;

        float_t += 0.05f;
        g_bird.y = BIRD_BODY_Y + (float)sin(float_t) * 5.0f;

        anim_counter++;
        if (anim_counter >= 8) {
            anim_counter = 0;
            g_bird.frame = (g_bird.frame + 1) % 3;
        }

        ready_timer++;
        if (ready_timer >= FPS * 3) {
            g_countdown = 0;
            g_scene = SCENE_GAME;
        } else {
            g_countdown = 3 - ready_timer / FPS;
            if (g_countdown < 1) g_countdown = 1;
        }

        if (input_space_triggered()) {
            audio_play_swoosh();
            g_scene = SCENE_GAME;
        }
        break;
    }

    case SCENE_GAME: {
        g_bg_offset -= BG_SCROLL_SPEED;
        g_ground_offset -= GROUND_SCROLL_SPEED;

        anim_counter++;
        if (anim_counter >= 5) {
            anim_counter = 0;
            g_bird.frame++;
            if (g_bird.frame > 2) g_bird.frame = 0;
        }

        bird_apply_gravity(&g_bird);

        if (input_space_triggered()) {
            bird_jump(&g_bird);
            audio_play_wing();
        }

        pipe_timer++;
        if (pipe_timer >= PIPE_SPAWN_INTERVAL) {
            pipe_timer = 0;
            spawn_pipe();
        }

        move_pipes();
        game_cleanup_offscreen_pipes();

        if (check_collision()) {
            audio_play_hit();
            audio_play_die();
            if (g_score > g_highscore) {
                g_highscore = g_score;
                save_highscore();
            }
            g_scene = SCENE_OVER;
            break;
        }

        if (check_score()) {
            g_score++;
            audio_play_point();
        }
        break;
    }

    case SCENE_OVER: {
        g_bg_offset -= BG_SCROLL_SPEED * 0.5f;
        g_ground_offset -= GROUND_SCROLL_SPEED * 0.5f;

        bird_apply_gravity(&g_bird);
        if (g_bird.y > SCREEN_H - GROUND_HEIGHT - BIRD_H) {
            g_bird.y = (float)(SCREEN_H - GROUND_HEIGHT - BIRD_H);
        }

        if (input_space_triggered()) {
            audio_play_swoosh();
            game_switch_scene(SCENE_START);
        }
        break;
    }
    }
}

void load_highscore(void)
{
    FILE *fp = fopen("data/highscore.txt", "r");
    g_highscore = 0;
    if (fp != NULL) {
        if (fscanf(fp, "%d", &g_highscore) != 1)
            g_highscore = 0;
        fclose(fp);
    }
}

void save_highscore(void)
{
    FILE *fp = fopen("data/highscore.txt", "w");
    if (fp != NULL) {
        fprintf(fp, "%d", g_highscore);
        fclose(fp);
    }
}
