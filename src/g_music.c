#include "g_music.h"
#include <stdio.h>
#include <string.h>

Mix_Music *mus = NULL;
char* bgm_file;
void play_sound(char* filename, int loop){
	Mix_Chunk  *gScratch = NULL;
	gScratch = Mix_LoadWAV(filename);
	if (gScratch == NULL)
	{
		slog("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
	}
	Mix_PlayChannel(-1, gScratch, loop);
}

void play_soundeffect(char* filename, int loop){
	Mix_Chunk  *chunk = NULL;
	chunk = Mix_LoadWAV(filename);
	if (chunk == NULL)
	{
		slog("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
	}
	Mix_PlayChannel(-1, chunk, loop);
}

void play_bgm(char* filename, int loop){
	mus = Mix_LoadMUS(filename);
	if (strcmp(filename,"../sounds/bosstheme.wav"))
		bgm_file = filename;
	if (mus == NULL)
	{
		slog("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
	}
	Mix_PlayMusic(mus, loop);
}
void stop_bgm(){
	Mix_HaltMusic();
}
void restart_bgm(){
	play_bgm(bgm_file, 0);
}