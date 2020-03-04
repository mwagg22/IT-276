#include "gf2d_sprite.h"
#include "g_entity.h"


void shooterenemy_think(Entity *self);
void shooterenemy_attack(Entity *self);
void update_shooterenemy_sprite(Entity *self);
void update_shooterenemy_ent(Entity *self);
void init_shooterenemy_ent(Entity *self, int ctr);
void shooterenemy_set_position(Entity *self, Vector2D position);
void shooterenemy_displacement(Entity *self, Vector2D position);
void shooterenemy_damage(Entity *self, int damage, Vector2D kick);
void create_shooterenemy_projectile(Entity *self, float speed, float dmg, int type);
void shooterenemy_death(Entity* self);