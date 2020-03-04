#include "gf2d_sprite.h"
#include "g_entity.h"



void update_hazard_ent(Entity *self);
void init_hazard_ent(Entity *self,int type);
void set_hazard_position(Entity *self, Vector2D position);
void hazard_displacement(Entity *self, Vector2D direction);

//hazard actions
void update_flamethrower_hazard(Entity* self);
void update_fallingrocks_hazard(Entity* self);
void update_slider_hazard(Entity* self);