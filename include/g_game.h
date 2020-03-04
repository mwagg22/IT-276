#include <SDL.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "gf2d_draw.h"
#include "simple_logger.h"
#include "g_gungirl.h"
#include "g_camera.h"
#include "g_level.h"
#include "g_collision.h"
#include "g_shooterenemy.h"
#include "g_rollerenemy.h"
#include "g_fireman.h"
#include "g_lookerenemy.h"
#include "g_hazards.h"
#include "g_menu.h"

#ifndef game_h
#define game_h
typedef enum {
	G_Intro = 0,
	G_Menu = 1,
	G_BossSelect = 2,
	G_Level = 3,
	G_Paused = 4
}game_state;

typedef struct game_states_S{
	game_state CurrentState;
	game_state PreviousState;
	main_menu *menu;
	select_screen *selectScreen;
	Level *currentLevel;
	Entity *controllerEntity;
	bool to_draw_entities;
	bool to_update;
	bool to_draw_map;
	bool transition;
}game_controller;

void init_game();
void set_game_state(game_state state);
void update_game(Camera *cam, Level *level, const Uint8 *keys);

#endif