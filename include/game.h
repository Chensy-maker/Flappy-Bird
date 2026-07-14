#ifndef GAME_H
#define GAME_H

#include "types.h"
#include <SDL2/SDL.h>

extern GameScene g_scene;
extern Bird      g_bird;
extern PipeList  g_pipes;
extern int       g_score;
extern int       g_highscore;
extern int       g_countdown;
extern int       g_running;
extern float     g_bg_offset;
extern float     g_ground_offset;

void game_init(void);
void game_update(void);
void game_switch_scene(GameScene s);
void game_free_pipes(void);
void game_cleanup_offscreen_pipes(void);

void load_highscore(void);
void save_highscore(void);

extern SDL_Renderer *g_renderer;

#endif
