#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "game.h"
#include "render.h"
#include "input.h"
#include "audio.h"

#define TARGET_DELTA (1000.0f / FPS)

int main(int argc, char *argv[])
{
    (void)argc; (void)argv;

    srand((unsigned int)time(NULL));

    if (render_init() != 0) {
        fprintf(stderr, "Failed to initialize render system.\n");
        return 1;
    }
    audio_init();
    game_init();

    Uint32 last_time = SDL_GetTicks();
    float accumulator = 0.0f;
    g_running = 1;

    while (g_running && !input_quit_requested()) {
        Uint32 now = SDL_GetTicks();
        float delta = (float)(now - last_time);
        last_time = now;

        if (delta > 100.0f) delta = TARGET_DELTA;
        accumulator += delta;

        while (accumulator >= TARGET_DELTA) {
            input_process_events();
            game_update();
            accumulator -= TARGET_DELTA;
        }

        render_draw();

        Uint32 frame_time = SDL_GetTicks() - now;
        if (frame_time < (Uint32)TARGET_DELTA) {
            SDL_Delay((Uint32)(TARGET_DELTA - frame_time));
        }
    }

    audio_quit();
    game_free_pipes();
    render_quit();

    printf("Thanks for playing! Final Score: %d\n", g_score);
    return 0;
}
