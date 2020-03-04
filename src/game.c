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

//change based on states
static game_controller *game = {0};

void init_game(){
	game->menu = init_menu_ent();
	game->PreviousState = G_Menu;
	game->CurrentState = G_Menu;
	game->controllerEntity = game->menu->cursor;
	game->transition = false;
}
void set_game_state(game_state state){
	game->PreviousState = game->CurrentState;
	game->transition = true;
	gf2d_clear_entity_manager();
	switch (state){
	case(G_Menu) : {
					   game->menu = init_menu_ent();
	}break;
	case(G_BossSelect) : {
					   game->selectScreen = init_select_screen();
	}break;
	case(G_Level) : {
						//load level file n stuff n level entity
	}break;
	}
	game->CurrentState = state;
	game->transition = false;
}

void update_game(Camera *cam, Level *level, const Uint8 *keys){
	switch (game->CurrentState){
	case(G_Menu) : {
					   get_menu_inputs(game->menu, keys);
	}break;
	case(G_BossSelect) : {

	}break;
	case(G_Level) : {
						entity_tile_collision(level->tiles);
						draw_tiles(level, cam);
	}break;
	}
	if (!game->transition){
		update_camera(cam);
		draw_entities(cam);
	}
}


int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    Sprite *sprite;
	gf2d_entity_manager_init(20);
	//Entity *mouse = gf2d_entity_new();
	Entity *player = gf2d_entity_new();
	Entity *shooter = gf2d_entity_new();
	Camera *cam = init_camera(player, 256, 240, vector2d(0, 0), vector2d(2100, 760));
	SDL_Rect rect;
    int mx,my;
    float mf = 0;
    //Sprite *mouse;
    Vector4D mouseColor = {255,255,255,255};
	
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
    gf2d_sprite_init(1024);
    SDL_ShowCursor(SDL_DISABLE);
    
	Level *level = load_level("../level1.json");
    /*demo setup*/
    sprite = gf2d_sprite_load_image("../images/backgrounds/bg_flat.png");
	init_gungirl_ent(player, 1);
	init_fireman_ent(shooter, 0);
    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        //SDL_GetMouseState(&mx,&my);
		gungirl_get_inputs(player, keys);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;
		
		//mouse->frame = (int)mf;
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
		Vector2D pos = vector2d(0, 0);
            
		update_game(game,cam, level,keys);
		rect.x = player->hitbox.x + player->hitbox.offsetx - cam->position.x;
		rect.y = player->hitbox.y + player->hitbox.offsety - cam->position.y;
		rect.w = player->hitbox.w;
		rect.h = player->hitbox.h;


		//gf2d_draw_rect(rect, mouseColor);
        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
		
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
