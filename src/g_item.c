#include "g_item.h"



void update_item_ent(Entity *self){
	self->frame += .1;
	if (self->frame > self->sprite->frames_per_line ){
		self->frame = 0;
	}
	if (!self->is_grounded){
		self->position.y += 1.5;
	}
	if (self->actionFrame > 0){
		self->actionFrame--;
	}
	if (self->actionFrame == 0){
		set_to_zero_ent(self->Ent_ID);
	}
}
void init_item_ent(Entity *self, Item_type type,int amount,int aliveFrame){
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
	self->color = vector4d(255, 255, 255, 255);
	self->health = amount;
	self->frame = 0;
	self->clip = 1;
	self->actionFrame = aliveFrame;
	self->type = ES_Item;
	self->itemType = type;
	self->is_grounded = false;
	self->update_ent = update_item_ent;
}
void item_set_position(Entity *self, Vector2D position){
	self->position = position;
}


void create_item(Item_type type, int amount, int aliveFrame, Vector2D position){
	Entity *item = gf2d_entity_new();
	init_item_ent(item, type, amount, aliveFrame);
	item_set_position(item, position);
}