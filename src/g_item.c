#include "g_item.h"



void update_item_ent(Entity *self){
	self->frame += .1;
	if (self->frame >= self->sprite->frames_per_line - 1){
		self->frame = 0;
	}
	if (!self->is_grounded){
		self->position.y += 1.5;
	}
	if (self->actionFrame > 0){
		self->actionFrame--;
	}
	if (self->actionFrame == 0){
		//destroy self
	}
}
void init_item_ent(Entity *self, Item_type type,int amount){
	switch (type){
	case I_Health:{
					  self->sprite = gf2d_sprite_load_all("../images/test/item/health.png", 16, 16, 1);;
	}break;
	case I_Bolt:{
					self->sprite = gf2d_sprite_load_all("../images/test/item/bolt.png", 16, 16, 1);;
	}break;
	case I_Energy:{
					  self->sprite = gf2d_sprite_load_all("../images/test/item/energy.png", 16, 16, 2);;
	}break;
	}
	set_hitbox(self, self->position.x, self->position.y, 16, 16, 0, 0);
	self->health = amount;
	self->frame = 0;
	self->clip = 1;
	self->actionFrame = -1;
	self->type = ES_Item;
}
void item_set_position(Entity *self, Vector2D position){
	self->position = position;
}