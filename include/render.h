#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>

int  render_init(void);
void render_quit(void);
void render_draw(void);

extern SDL_Window   *g_window;
extern SDL_Texture  *tex_bg;
extern SDL_Texture  *tex_ground;
extern SDL_Texture  *tex_bird[3];
extern SDL_Texture  *tex_pipe_down;
extern SDL_Texture  *tex_pipe_up;
extern SDL_Texture  *tex_title;
extern SDL_Texture  *tex_ready;
extern SDL_Texture  *tex_game_over;
extern SDL_Texture  *tex_numbers[10];
extern SDL_Texture  *tex_score_panel;
extern SDL_Texture  *tex_new;

#endif
