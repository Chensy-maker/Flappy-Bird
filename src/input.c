#include "input.h"

static bool space_down = false;
static bool space_prev = false;
static bool quit_req = false;

void input_process_events(void)
{
    SDL_Event e;
    space_prev = space_down;

    while (SDL_PollEvent(&e)) {
        switch (e.type) {
        case SDL_QUIT:
            quit_req = true;
            break;
        case SDL_KEYDOWN:
            if (e.key.keysym.sym == SDLK_SPACE)
                space_down = true;
            if (e.key.keysym.sym == SDLK_ESCAPE)
                quit_req = true;
            break;
        case SDL_KEYUP:
            if (e.key.keysym.sym == SDLK_SPACE)
                space_down = false;
            break;
        }
    }
}

bool input_space_pressed(void)
{
    return space_down;
}

bool input_space_triggered(void)
{
    return space_down && !space_prev;
}

bool input_quit_requested(void)
{
    return quit_req;
}
