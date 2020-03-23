#include "gf2d_sprite.h"
#include "g_entity.h"


void metalman_think(Entity *self);
void metalman_attack(Entity *self);
void metalman_jump(Entity *self);
void update_metalman_sprite(Entity *self);
void update_metalman_ent(Entity *self);
void init_metalman_ent(Entity *self, int ctr);
void metalman_set_position(Entity *self, Vector2D position);
void metalman_displacement(Entity *self, Vector2D position);
void metalman_damage(Entity *self, int damage, Vector2D kick);
void create_metalman_projectile(Entity *self, float speed, float dmg, int type);
void metalman_death(Entity* self);
void metalman_reset(Entity *self);