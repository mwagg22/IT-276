#include "gf2d_sprite.h"
#include "g_entity.h"


void update_text(Entity *self,int aliveframe,int size,char* string,Vector4D color,Vector2D position,bool follow);
void init_text(Entity *self,int aliveframe,int size,char* string,char*font,Vector4D color,Vector2D position, bool follow);
void text_set_position(Entity *self, Vector2D position);
void draw_text(Entity *self);
void update_text_ent(Entity *self);