#include "gf2d_sprite.h"
#include "g_entity.h"



void update_effect_ent(Entity *self);
void init_effect_ent(Entity *self, int type,int aliveFrame);
void effect_set_position(Entity *self, Vector2D position);
void create_effect(Vector2D position, int type, int aliveFrame, Effects data);