#include "gf2d_sprite.h"
#include "g_entity.h"



void update_projectile_ent(Entity *self);
void init_projectile_ent(Entity *self, int actionFrame);
void set_projectile_position(Entity *self, Vector2D position);
void projectile_displacement(Entity *self, Vector2D direction);
void create_projectile(Vector2D position,Vector2D direction);