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
#include "g_game.h"
#include "simple_json_object.h"
#include "g_bar.h"
#include "g_music.h"
//change based on states
game_controller *game;
const Uint8 * keys;
SDL_Event event;
void init_game(){
	game->menu=init_menu_ent();
	game->data = (game_data*)malloc(sizeof(game_data));
	game->data->bossdata = (int*)gfc_allocate_array(sizeof(int), 4);
	game->data->weapondata = (int*)gfc_allocate_array(sizeof(int), 4);
	memset(game->data->bossdata, 0, sizeof(int)* 4);
	memset(game->data->weapondata, 0, sizeof(int)* 4);
	game->cam=init_camera(NULL, 256, 240, vector2d(0, 0), vector2d(256, 240));
	game->PreviousState = G_Menu;
	game->CurrentState = G_Menu;
	game->controllerEntity = game->menu->cursor;
	game->transition = false;
	game->update = false;
	game->can_input = true;
	game->updateType = 0;
	game->pause = false;
	game->boss_state = true;
}
void set_update_type(int type){
	game->updateType = type;
}
void set_game_camera_bounds(Vector2D leftbounds, Vector2D rightbounds){
	set_bounds(game->cam, leftbounds, rightbounds);
}
void set_game_state(game_state state,int level){
	game->PreviousState = game->CurrentState;
	game->transition = true;
	game->update = false;
	gf2d_clear_entity_manager();
	switch (state){
	case(G_Menu) : {
					   game->menu = init_menu_ent();
					   set_game_camera_bounds(vector2d(0, 0), vector2d(256, 240));
					   
	}break;
	case(G_BossSelect) : {
					   game->selectScreen = init_select_screen();
					   set_game_camera_bounds(vector2d(0, 0), vector2d(256, 240));
					   stop_bgm();
	}break;
	case(G_Level) : {
						//load level file n stuff n level entity
						switch (level){
						case(0) : {
								//fireman
									  game->currentLevel = load_level("../levels/firemanstage.json");
									  play_bgm("../sounds/metalman.wav", 0);
						}break;
						case(1) : {
								//test
									  game->currentLevel = load_level("../levels/testroom.json");
						}break;
						case(2) : {
								//metalman
									  game->currentLevel = load_level("../levels/bosstest.json");
						}break;
						case(3) : {
									  //bubbleman
									  
						}break;

						}
						game->boss_state = true;
						game->controllerEntity = get_player_entity();
						game->pauseMenu = init_pause_screen();
						set_input_control(0);
						set_target(game->cam, game->controllerEntity, 1);
	}break;
	}
	game->CurrentState = state;
	game->transition = false;

}

void load_save(){
	//check if file if not return
	//else read json data and fill in data of game_data
	SJson *file,*value;
	file = sj_load("../gamedata.sav");
	if (file == NULL){
		slog("file not found");
		return;
	}
	//get boss data and player weapon data
	value = sj_object_get_value(file, "bossdata");

	game->data->bossdata = sj_array_return(value);
	sj_echo(value);
	value = sj_object_get_value(file, "weapondata");
	sj_echo(value);
	game->data->weapondata = sj_array_return(value);
	set_game_state(G_BossSelect, 0);
}

void save_game(){
	SJson *file,*array,*array2,*data;
	file = sj_object_new();
	array=sj_array_new();
	array2 = sj_array_new();
	
	for (int i = 0; i < 4; i++){
		data = sj_new_int(game->data->bossdata[i]);
		sj_array_append(array, data);
	}
	
	for (int i = 0; i < 4; i++){
		data = sj_new_int(game->data->weapondata[i]);
		sj_array_append(array2, data);
	}

	sj_object_insert(file, "bossdata", array);
	sj_object_insert(file, "weapondata", array2);
	sj_save(file, "../gamedata.sav");
}

void set_input_control(int control){
	game->can_input = control;
}
void update_game_data(char* key,int index ,int value){
	if (!strcmp(key, "bossdata")){
		game->data->bossdata[index] = value;
	}
	if (!strcmp(key, "weapondata")){
		game->data->weapondata[index] = value;
	}
}

game_state return_game_state(){
	return game->CurrentState;
}
Camera* get_game_camera(){
	return game->cam;
}
game_controller* return_game_controller(){
	return game;
}
void set_game_camera_start_bounds(Vector2D leftbounds, Vector2D rightbounds){
	game->cameraTopBound = leftbounds;
	game->cameraBottomBound = rightbounds;
}
void reset_game_camera(){
	set_game_camera_bounds(game->cameraTopBound, game->cameraBottomBound);
}
void update_boss_state(){
	if (game->boss_state == true){
		game->boss_state = false;
		game->controllerEntity->victory(game->controllerEntity);
		game->can_input = false;
		play_bgm("../sounds/victory.wav", 0);
	}
	else
		return;
}
void update_game(const Uint8 *keys){
	switch (game->CurrentState){
	case(G_Menu) : {
					   if (game->can_input)
							get_menu_inputs(game->menu, keys);
	}break;
	case(G_BossSelect) : {
							 if (game->update == false){
								 for (int i = 0; i < 4; i++){
									 if (game->data->bossdata[i] == 1){
										 update_select_ents(game->selectScreen->icon_items[i], ES_Dead);
									 }
								 }
								 game->update = true;
							 }
							 if (game->can_input)
								get_select_inputs(game->selectScreen, keys);
	}break;
	case(G_Level) : {
						if (game->update == false){
							for (int i = 0; i < 4; i++){
								if (game->data->weapondata[i] == 0)
									continue;
								gungirl_get_ability(game->controllerEntity,i);
							}
							game->update = true;
						}
						if (game->pause){
							set_update_type(2);
							get_pause_inputs(game->pauseMenu, keys);
						}
						else if (game->can_input)
							gungirl_get_inputs(game->controllerEntity,keys);
						entity_tile_collision(game->currentLevel->tiles);
						
						
						
	}break;
	}
	if (!game->transition){
		update_camera(game->cam);
		update_all_entities(game->updateType);
		if (game->CurrentState == G_Level){
			draw_tiles(game->currentLevel, game->cam);
			draw_entities(game->cam);
			draw_ui(game->cam);
		}
		else
			draw_entities(game->cam);
		
	}
}
void clear_key(){
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym){
			case SDLK_UP: {

			}
				break;
			case SDLK_DOWN:{

			}break;
			case SDLK_LEFT:{

			}break;
			case SDLK_RIGHT:{

			}break;
			case SDLK_a:{

			}break;
			case SDLK_s:{

			}break;
			case SDLK_d:{

			}break;
			}break;
		case SDL_KEYUP:
			switch (event.key.keysym.sym){
			case SDLK_UP: {

			}
				break;
			case SDLK_DOWN:{

			}break;
			case SDLK_LEFT:{

			}break;
			case SDLK_RIGHT:{

			}break;
			case SDLK_a:{

			}break;
			case SDLK_s:{

			}break;
			case SDLK_d:{

			}break;
			}break;
		}
	}
}

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
	
	game = (game_controller*)malloc(sizeof(game_controller));
    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        256,
        240,
        256,
        240,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
	gf2d_entity_manager_init(200);
    gf2d_sprite_init(1024);

    SDL_ShowCursor(SDL_DISABLE);

	init_game();
    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        gf2d_graphics_clear_screen();// clears drawing buffers
		Vector2D pos = vector2d(0, 0);
				
		update_game(keys);

        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
		
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
