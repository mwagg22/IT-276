#include "gf2d_sprite.h"
#include "g_entity.h"


void collision_check(Entity *ents, Uint32 entity_max);
bool check_collision(Entity *self, Entity *other);
bool check_hitbox_collision(Entity *hitlist, Entity *other);
void handle_hitbox_collision(Entity *self, Entity *other, Vector2D kick);
void handle_projectile_collision(Entity *self, Entity *other);
void handle_collision(Entity *self, Entity *other);
