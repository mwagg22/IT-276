#include "gf2d_sprite.h"
#include "g_entity.h"


void rollerenemy_think(Entity *self);
void rollerenemy_attack(Entity *self);
void update_rollerenemy_sprite(Entity *self);
void update_rollerenemy_ent(Entity *self);
void init_rollerenemy_ent(Entity *self, int ctr);
void rollerenemy_set_position(Entity *self, Vector2D position);
void rollerenemy_displacement(Entity *self, Vector2D position);
void rollerenemy_damage(Entity *self, int damage, Vector2D kick);
void rollerenemy_death(Entity* self);
void rollerenemy_reset(Entity* self);