#include "gf2d_sprite.h"
#include "g_entity.h"


void fireman_think(Entity *self);
void fireman_attack(Entity *self);
void update_fireman_sprite(Entity *self);
void update_fireman_ent(Entity *self);
void init_fireman_ent(Entity *self, int ctr);
void fireman_set_position(Entity *self, Vector2D position);
void fireman_displacement(Entity *self, Vector2D position);
void fireman_damage(Entity *self, int damage, Vector2D kick);
void create_fireman_projectile(Entity *self, float speed, float dmg, int type);
void fireman_death(Entity* self);