#ifndef Music_Struct
#define Music_Struct

#include <SDL.h>
#include <stdlib.h>
#include <SDL_mixer.h>
#include <stdbool.h>
#include "simple_logger.h"

void gf2d_sound_manager_init(Uint32 sound_max);
void gf2d_sound_manager_close();
Mix_Chunk *gf2d_sound_new();

void play_sound(char* filename, int loop);
void play_soundeffect(char* filename, int loop);
void play_bgm(char* filename, int loop);
void stop_bgm();
void restart_bgm();
#endif