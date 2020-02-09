#include "gf2d_sprite.h"
#include "g_entity.h"


void sword_think(Entity *self);
void sword_attack(Entity *self);
void sword_ai_think(Entity *self);
void sword_special(Entity *self);
void sword_block(Entity *self);
void update_sword_model(Entity *self);
void update_sword_ent(Entity *self);
void init_sword_ent(Entity *self,int ctr);
void sword_set_position(Entity *self, Vector2D position);
void sword_get_inputs(Entity *self, const Uint8 * keys, float delta);