#include "input.h"

static bool space_down = false, space_prev = false;
static bool left_down = false, left_prev = false;
static bool right_down = false, right_prev = false;
static bool pause_down = false, pause_prev = false;
static bool quit_req = false;

void input_process_events(void)
{
    space_prev = space_down;
    left_prev = left_down;
    right_prev = right_down;
    pause_prev = pause_down;
    SDL_Event e;

    while (SDL_PollEvent(&e)) {
        switch (e.type) {
        case SDL_QUIT:
            quit_req = true; break;
        case SDL_KEYDOWN:
            switch (e.key.keysym.sym) {
            case SDLK_SPACE:  space_down = true; break;
            case SDLK_LEFT:   left_down = true; break;
            case SDLK_RIGHT:  right_down = true; break;
            case SDLK_p:
            case SDLK_PAUSE:  pause_down = true; break;
            case SDLK_ESCAPE: quit_req = true; break;
            default: break;
            }
            break;
        case SDL_KEYUP:
            switch (e.key.keysym.sym) {
            case SDLK_SPACE:  space_down = false; break;
            case SDLK_LEFT:   left_down = false; break;
            case SDLK_RIGHT:  right_down = false; break;
            case SDLK_p:
            case SDLK_PAUSE:  pause_down = false; break;
            default: break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (e.button.button == SDL_BUTTON_LEFT)
                space_down = true;
            break;
        case SDL_MOUSEBUTTONUP:
            if (e.button.button == SDL_BUTTON_LEFT)
                space_down = false;
            break;
        }
    }
}

bool input_space_pressed(void)    { return space_down; }
bool input_space_triggered(void)  { return space_down && !space_prev; }
bool input_left_triggered(void)   { return left_down && !left_prev; }
bool input_right_triggered(void)  { return right_down && !right_prev; }
bool input_pause_triggered(void)  { return pause_down && !pause_prev; }
bool input_quit_requested(void)   { return quit_req; }
