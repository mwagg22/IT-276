#include "gf2d_sprite.h"
#include "g_entity.h"

typedef struct{
	SDL_Rect bounds;
	Sprite *background;
	Sprite *tiles;
}Level;

void gf2d_level_manager_init(Uint32 entity_max);

void free_ent_manager(int i);
void load_level(Level *level);
