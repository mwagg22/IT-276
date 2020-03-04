#include "gf2d_sprite.h"
#include "g_entity.h"


void lookerenemy_think(Entity *self);
void lookerenemy_attack(Entity *self);
void update_lookerenemy_sprite(Entity *self);
void update_lookerenemy_ent(Entity *self);
void init_lookerenemy_ent(Entity *self, int ctr);
void lookerenemy_set_position(Entity *self, Vector2D position);
void lookerenemy_displacement(Entity *self, Vector2D position);
void lookerenemy_damage(Entity *self, int damage, Vector2D kick);
void create_lookerenemy_projectile(Entity *self, float speed, float dmg, int type);
void lookerenemy_death(Entity* self);