#include "gf2d_sprite.h"
#include "g_entity.h"

typedef struct bar_S{
	//0 health 1 weapons
	int type;
}bar;

void update_bar_ent(Entity *self);
void init_bar_ent(Entity *self, Item_type type,int amount,int aliveFrame);
void bar_set_position(Entity *self, Vector2D position);
void create_bar(Item_type type, int amount, int aliveFrame, Vector2D position);
void draw_bar(Entity* self);
void create_bar_ent(EntityType type);