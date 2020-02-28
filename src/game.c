#include <SDL.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "gf2d_draw.h"
#include "simple_logger.h"
#include "g_gungirl.h"
#include "g_camera.h"
#include "g_level.h"
#include "g_collision.h"
void update_game(Camera *cam,Level *level){
	update_camera(cam);
	//slog("camera position:%f,%f",cam->position.x, cam->position.y);
	draw_tiles(level, cam);
	draw_entities(cam);
}

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    Sprite *sprite;
	gf2d_entity_manager_init(20);
	Entity *mouse = gf2d_entity_new();
	Entity *player = gf2d_entity_new();
	Camera *cam = init_camera(player, 480, 320, vector2d(0, 0), vector2d(2100, 500));
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
        480,
        320,
        480,
        320,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    SDL_ShowCursor(SDL_DISABLE);
    
	Level *level = load_level("../level1.json");
    /*demo setup*/
    sprite = gf2d_sprite_load_image("../images/backgrounds/bg_flat.png");
	mouse->sprite_list.idle = gf2d_sprite_load_all("../images/sprite.png", 100, 100, 10);
	mouse->sprite = mouse->sprite_list.idle;
	init_gungirl_ent(player, 1);
	mouse->color = mouseColor;
    //mouse = gf2d_sprite_load_all("../images/sprite.png",100,100,10);
    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        //SDL_GetMouseState(&mx,&my);
		gungirl_get_inputs(player, keys);
		check_tile_ahead(player, level->tiles);
		check_tile_collision(player, level->tiles);
		update_gungirl_ent(player);
		mouse->position = vector2d(5, 5);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;
		
		mouse->frame = (int)mf;
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
		Vector2D pos = vector2d(0, 0);
             //gf2d_sprite_draw_image(sprite,pos,cam->position);
            
            //UI elements last
            /*gf2d_sprite_draw(
                mouse,
                vector2d(mx,my),
                NULL,
                NULL,
                NULL,
				NULL,
                &mouseColor,
                (int)mf);*/
		update_game(cam, level);
		rect.x = player->hitbox.x + player->hitbox.offsetx - cam->position.x;
		rect.y = player->hitbox.y + player->hitbox.offsety - cam->position.y;
		rect.w = player->hitbox.w;
		rect.h = player->hitbox.h;


		gf2d_draw_rect(rect, mouseColor);
        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
		
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
