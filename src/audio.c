#include "audio.h"
#include <SDL2/SDL_mixer.h>
#include <stdio.h>

static Mix_Chunk *sfx_wing = NULL;
static Mix_Chunk *sfx_point = NULL;
static Mix_Chunk *sfx_hit = NULL;
static Mix_Chunk *sfx_die = NULL;
static Mix_Chunk *sfx_swoosh = NULL;

void audio_init(void)
{
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
        fprintf(stderr, "Mix_OpenAudio error: %s\n", Mix_GetError());
        return;
    }
    Mix_AllocateChannels(16);

    sfx_wing   = Mix_LoadWAV("assets/sounds/sfx_wing.ogg");
    sfx_point  = Mix_LoadWAV("assets/sounds/sfx_point.ogg");
    sfx_hit    = Mix_LoadWAV("assets/sounds/sfx_hit.ogg");
    sfx_die    = Mix_LoadWAV("assets/sounds/sfx_die.ogg");
    sfx_swoosh = Mix_LoadWAV("assets/sounds/sfx_swooshing.ogg");

    if (!sfx_wing)   fprintf(stderr, "Failed loading sfx_wing: %s\n", Mix_GetError());
    if (!sfx_point)  fprintf(stderr, "Failed loading sfx_point: %s\n", Mix_GetError());
    if (!sfx_hit)    fprintf(stderr, "Failed loading sfx_hit: %s\n", Mix_GetError());
    if (!sfx_die)    fprintf(stderr, "Failed loading sfx_die: %s\n", Mix_GetError());
    if (!sfx_swoosh) fprintf(stderr, "Failed loading sfx_swoosh: %s\n", Mix_GetError());
}

void audio_quit(void)
{
    Mix_FreeChunk(sfx_wing);
    Mix_FreeChunk(sfx_point);
    Mix_FreeChunk(sfx_hit);
    Mix_FreeChunk(sfx_die);
    Mix_FreeChunk(sfx_swoosh);
    Mix_CloseAudio();
}

void audio_play_wing(void)   { if (sfx_wing)   Mix_PlayChannel(-1, sfx_wing, 0); }
void audio_play_point(void)  { if (sfx_point)  Mix_PlayChannel(-1, sfx_point, 0); }
void audio_play_hit(void)    { if (sfx_hit)    Mix_PlayChannel(-1, sfx_hit, 0); }
void audio_play_die(void)    { if (sfx_die)    Mix_PlayChannel(-1, sfx_die, 0); }
void audio_play_swoosh(void) { if (sfx_swoosh) Mix_PlayChannel(-1, sfx_swoosh, 0); }
