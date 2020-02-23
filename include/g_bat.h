#include "gf2d_sprite.h"
#include "g_entity.h"


void bat_think(Entity *self);
void bat_attack(Entity *self);
void update_bat_sprite(Entity *self);
void update_bat_ent(Entity *self);
void init_bat_ent(Entity *self, int ctr);
void bat_set_position(Entity *self, Vector2D position);
void bat_displacement(Entity *self, Vector2D position);