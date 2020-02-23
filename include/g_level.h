#include "gf2d_sprite.h"
#include "g_entity.h"
#include "simple_json.h"

typedef struct{
	SDL_Rect bounds;
	Sprite *background;
	char** tiles;
	int width;
	int height;
}Level;

Level* load_level(char* filename);
char** return_tiles(Level* level);
int return_level_width(Level* level);
int return_level_height(Level* level);