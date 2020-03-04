#include "gf2d_sprite.h"
#include "g_entity.h"



void update_item_ent(Entity *self);
void init_item_ent(Entity *self, Item_type type,int amount,int aliveFrame);
void item_set_position(Entity *self, Vector2D position);
void create_item(Item_type type, int amount, int aliveFrame, Vector2D position);