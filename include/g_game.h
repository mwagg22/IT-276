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

typedef struct game_data_S{
	int* bossdata;
	int* weapondata;
}game_data;

typedef struct game_states_S{
	game_state CurrentState;
	game_state PreviousState;
	main_menu *menu;
	select_screen *selectScreen;
	pause_menu *pauseMenu;
	Level *currentLevel;
	Entity *controllerEntity;
	Camera *cam;
	game_data *data;
	bool update;
	bool to_draw_entities;
	bool to_update;
	bool to_draw_map;
	bool transition;
	bool have_save;
	bool can_input;
	bool pause;
	bool boss_state;
	int updateType;
	Vector2D cameraTopBound;
	Vector2D cameraBottomBound;

}game_controller;

void init_game();
void set_game_state(game_state state,int level);
void update_game(const Uint8 *keys);
game_state return_game_state();
void load_save();
void save_game();
game_controller* return_game_controller();
void update_game_data(char* key, int index, int value);
void set_input_control(bool control);
void set_game_camera_bounds(Vector2D leftbounds, Vector2D rightbounds);
void set_game_camera_start_bounds(Vector2D leftbounds, Vector2D rightbounds);
void reset_game_camera();
Camera* get_game_camera();
void set_update_type(int type);
void update_boss_state();
void clear_key();
#endif