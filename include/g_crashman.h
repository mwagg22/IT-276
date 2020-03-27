#include "gf2d_sprite.h"
#include "g_entity.h"


void crashman_think(Entity *self);
void crashman_attack(Entity *self);
void crashman_jump(Entity *self);
void update_crashman_sprite(Entity *self);
void update_crashman_ent(Entity *self);
void init_crashman_ent(Entity *self, int ctr);
void crashman_set_position(Entity *self, Vector2D position);
void crashman_displacement(Entity *self, Vector2D position);
void crashman_damage(Entity *self, int damage, Vector2D kick);
void create_crashman_projectile(Entity *self, float speed, float dmg, int type);
void crashman_death(Entity* self);
void crashman_reset(Entity *self);