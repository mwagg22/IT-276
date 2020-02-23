#include "gf2d_sprite.h"
#include "g_entity.h"


void shooterenemy_think(Entity *self);
void shooterenemy_attack(Entity *self);
void update_shooterenemy_sprite(Entity *self);
void update_shooterenemy_ent(Entity *self);
void init_shooterenemy_ent(Entity *self, int ctr);
void shooterenemy_set_position(Entity *self, Vector2D position);
void shooterenemy_displacement(Entity *self, Vector2D position);