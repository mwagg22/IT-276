#include "g_effects.h"

void update_effect_ent(Entity *self){
	self->frame += .1;
	if (self->frame >= self->sprite->frames_per_line - 1){
		if (self->actionFrame == -1){
			set_to_zero_ent(self->Ent_ID);
			return;
		}
		else
			self->frame = 0;
	}
	if (self->actionFrame > 0){
		self->actionFrame--;
	}
	if (self->actionFrame == 0){
		set_to_zero_ent(self->Ent_ID);
	}
}
void init_effect_ent(Entity *self, int type,int aliveFrame){
	switch (type){
	case 0:{
					  self->sprite = gf2d_sprite_load_all("../images/test/effect/explosion.png", 24, 24, 5);
	}break;
	case 1:{
					   self->sprite = gf2d_sprite_load_all("../images/test/effect/boom.png", 56, 56, 15);
	}break;
	case 2:{
					self->sprite = gf2d_sprite_load_all("../images/test/effect/explosion.png", 24, 24, 5);
	}break;
	}
	self->color = vector4d(255, 255, 255, 255);
	self->frame = 0;
	self->clip = 0;
	self->actionFrame = aliveFrame;
	self->type = ES_Effect;
	self->setPosition = effect_set_position;
	self->update_ent = update_effect_ent;
}
void effect_set_position(Entity *self, Vector2D position){
	self->position = position;
}

void create_effect(Vector2D position, int type, int aliveFrame,Effects data){
	Entity* effect = gf2d_entity_new();
	init_effect_ent(effect, type, aliveFrame);
	effect->effect_data = data;
	effect->sprite->sprite_offsetx = data.offset.x;
	effect->sprite->sprite_offsety = data.offset.y;
	if (type==1)
		play_soundeffect("../sounds/boom.wav", 0);
	if (data.centered==true){
		effect->sprite->sprite_offsetx = (int)(effect->sprite->frame_w/2);
		effect->sprite->sprite_offsety = (int)(effect->sprite->frame_h / 2);
	}
	effect_set_position(effect, position);
}