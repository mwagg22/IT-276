#include "gf2d_sprite.h"
#include "g_entity.h"

typedef struct{
	Sprite *tile;
	Vector2D position;
}Tiles;

void gf2d_tile_manager_init(Uint32 entity_max);

void free_tile_manager(int i);
void load_tile(Tiles *level);
void draw_tile(Tiles *tile);