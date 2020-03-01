#include "g_projectile.h"



void update_projectile_ent(Entity *self){
	self->frame += .1;
	if (self->frame >= self->sprite->frames_per_line - 1){
		self->frame = 0;
	}
	if (self->proj_data.aliveFrame > 0){
		self->proj_data.aliveFrame--;
	}
	if (self->proj_data.aliveFrame == 0){
		//destroy self
	}
	projectile_displacement(self,self->proj_data.direction);
	//check if inside camera bounds
}
void init_projectile_ent(Entity *self, Item_type type,int amount){
	switch (type){
	case I_Health:{
					  self->sprite ;
	}break;
	case I_Bolt:{
					self->sprite ;
	}break;
	case I_Energy:{
					  self->sprite ;
	}break;
	}
	self->health = amount;
	self->frame = 0;
	self->clip = 1;
	self->type = ES_Item;
}
void set_projectile_position(Entity *self, Vector2D position){
	self->position = position;
}
void projectile_displacement(Entity *self, Vector2D direction){
	self->position.x += direction.x;
	self->position.y += direction.y;
}