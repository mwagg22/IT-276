#include "gf2d_sprite.h"
#include "g_entity.h"


void gungirl_think(Entity *self);
void gungirl_attack(Entity *self);
void gungirl_special(Entity *self);
void gungirl_dash(Entity *self);
void gungirl_jump(Entity *self);
void gungirl_damage(Entity *self, int damage,Vector2D kick);
void update_gungirl_sprite(Entity *self);
void update_gungirl_ent(Entity *self);
void init_gungirl_ent(Entity *self, int ctr);
void gungirl_set_position(Entity *self, Vector2D position);
void gungirl_displacement(Entity *self, Vector2D position);
void gungirl_get_inputs(Entity *self, const Uint8 * keys);
void create_gungirl_projectile(Entity *self, float speed, float dmg, int type, float xpos, float ypos, int angleop, int effectcol, int stick);
void gungirl_get_ability(Entity* self, int index);
int* init_weapons_list();
void gungirl_update_list(Entity* self,int amount);
void gungirl_transition(Entity *self);
void gungirl_victory(Entity *self);
void gungirl_death(Entity *self);
void gungirl_reset(Entity *self);
void gungirl_respawn(Entity* self);