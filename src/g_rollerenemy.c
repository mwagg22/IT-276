#include "g_rollerenemy.h"
#include "simple_logger.h"

void rollerenemy_think(Entity *self){
	float attack_range = 6.0;
	Entity *target = get_player_entity();
	if (target){
		if (vector2d_magnitude_between(target->position, self->position) > attack_range){
			self->action = none;
		}
		else{
			self->action = attack;
		}

	}
	if (self->in_action == false){
		switch (self->action){
		case attack:
			if (self->can_attack == true){
				self->attack(self);
			}
			break;
		case movement:
			break;
		default:
			break;
		}
	}
}
void rollerenemy_attack(Entity *self){
	switch (self->attacknum){
	case(0) :
		if (self->is_grounded == true){
			self->can_attack = false;
			self->in_action = true;
			self->state = ES_Attacking;
			self->update_sprite(self);
		}
		break;
	default:
		//nothing?
		break;
	}
}
void update_rollerenemy_sprite(Entity *self){
	switch (self->state){

	case(ES_Idle) : {
		slog("idle time");
			self->frame = 0;
			self->sprite = self->sprite_list.idle;
	}break;
	case(ES_Attacking) : {

	}break;
	case(ES_Dying) : {
			self->sprite = self->sprite_list.dying;
	}break;
	}


}
void update_rollerenemy_ent(Entity *self){
	self->frame += 0.1;
	switch (self->dir){
	case(Right) :
	{
					self->flip = vector2d(0, 0);
	}break;
	case(Left) :
	{
				   self->flip = vector2d(1, 0);
	}break;
	}
	if (!self->is_grounded){
		self->position.y += 1.5;  //might change to gravity
	}
	if (self->position.y >= 500){
		self->is_grounded = true;
		if (self->state == ES_Jump){
			self->state = ES_Idle;
			self->maxjump = 1;
			self->jumpFrame = 0;
			self->update_sprite(self);
		}
	}
	//end frames
	if (self->frame > self->sprite->frames_per_line - 1){
		if (self->in_attack){
			self->can_attack = true;
			self->in_action = false;
			self->state = ES_Idle;
			self->update_sprite(self);

		}
		else{
			self->frame = 0;
			self->in_action = false;
		}
	}

}
void init_rollerenemy_ent(Entity *self, int ctr){
	self->state = ES_Idle;
	//self->update_sprite = update_rollerenemy_sprite;
	self->controlling = ctr;
	self->frame = 0;
	self->position = vector2d(100, 500);
	self->color = vector4d(255, 255, 255, 200);
	self->sprite_list.idle = gf2d_sprite_load_all("../images/test/idle.png", 100, 100, 1);
	self->sprite_list.run = gf2d_sprite_load_all("../images/test/run.png", 100, 100, 4);
	self->sprite_list.jump = gf2d_sprite_load_all("../images/test/jump.png", 100, 100, 2);
	self->sprite_list.dash = gf2d_sprite_load_all("../images/test/dash.png", 100, 100, 1);
	//self->sprite_list->dying;
	self->sprite_list.attack1 = gf2d_sprite_load_all("../images/test/attack1.png", 100, 100, 3);
	self->sprite_list.attack2 = gf2d_sprite_load_all("../images/test/attack2.png", 100, 100, 8);
	self->sprite_list.attack3 = gf2d_sprite_load_all("../images/test/attack3.png", 100, 100, 3);
	//self->sprite_list->special1;
	self->think = rollerenemy_think;
	self->movementspeed = 1;
	self->dashspeed = 3;
	self->maxjump = 1;
	self->update_sprite = update_rollerenemy_sprite;
	self->is_grounded = true;
	self->can_attack = true;
	self->attack_trigger = false;
	self->is_held = false;
	self->action = none;
	self->update_ent = update_rollerenemy_ent;
	self->sprite = self->sprite_list.idle;
}
void rollerenemy_set_position(Entity *self, Vector2D position){
	self->position = position;
}
void rollerenemy_displacement(Entity *self, Vector2D position){
	self->position.x += position.x;
	self->position.y += position.y;
}