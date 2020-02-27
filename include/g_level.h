#include "gf2d_sprite.h"
#include "g_entity.h"
#include "simple_json.h"
#include "g_camera.h"

#ifndef level_s
#define level_s
typedef struct{
	SDL_Rect bounds;
	Sprite *background;
	Sprite *tilesheet;
	Vector4D color;
	int** tiles;
	int width;
	int height;
	int tilewidth;
	int tileheight;
	int tileimages;
}Level;
#endif

Level* load_level(char* filename);
void draw_tiles(Level* level, Camera* cam);
int** return_tiles(Level* level);
int return_level_width(Level* level);
int return_level_height(Level* level);
