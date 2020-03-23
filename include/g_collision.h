#include "gf2d_sprite.h"
#include "g_entity.h"



void collision_check(Entity *ents, Uint32 entity_max);
bool check_collision(Entity *self, Entity *other);
bool check_hitbox_collision(Entity *hitlist, Entity *other);
void handle_hitbox_collision(Entity *self, Entity *other, Vector2D kick);
void handle_projectile_collision(Entity *self, Entity *other, Vector2D kick);
void handle_collision(Entity *self, Entity *other);
void handle_hazard_collision(Entity *self, Entity *other, Vector2D kick);
void check_tile_collision(Entity *self, int** tiles);
void check_tile_ahead(Entity *self, int** tiles);
bool check_empty_tile(Entity *self, int**tiles);
void handle_tile_collision(Entity *self,int x,int y);
void handle_item_collision(Entity *self, Entity *other);
void handle_transition_collision(Entity *self, Entity *other);