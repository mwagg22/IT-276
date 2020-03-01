#include "g_rollerenemy.h"
#include "simple_logger.h"
#include <math.h>
void rollerenemy_think(Entity *self){
	if (self->state != ES_Dead){
		float attack_range = 50.0;
		Entity *target = get_player_entity();
		//slog("think");
		if (target->_inuse){
			if (vector2d_magnitude_between(target->position, self->position) > attack_range){
				self->action = none;
				//slog("not in range");
			}
			else{
				//slog("in range");
				int rdm = (rand() % 11);
				if (rdm < 8)
					self->action = attack;
				else
					self->action = none;
			}

		}
		if (self->in_action == false){

			switch (self->action){

			case attack:
				if (self->can_attack == true){
					flip(self, target->position);
					self->attack(self);
				}
				break;
			case movement:
				break;
			case none:{
						  //flip(self, target->position);
						  self->actionFrame = (rand() % 11);
						  self->in_action = true;
						  self->state = ES_Idle;
						  self->update_sprite(self);
			}
				break;
			default:
				break;
			}
		}
	}
}
void rollerenemy_attack(Entity *self){
	switch (self->attacknum){
	case(0) :
		if (self->is_grounded == true){
			//slog("roller attack");
			self->actionFrame = 120;
			self->can_attack = false;
			self->in_action = true;
			self->in_attack = true;
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
						//slog("idle time");

						self->frame = 0;
						self->sprite = self->sprite_list.idle;
	}break;
	case(ES_Jump) : {
						//slog("jump roller time");
						self->frame = 0;
						self->sprite = self->sprite_list.idle;
	}break;
	case(ES_Attacking) : {
							 //slog("roller time");
							 self->frame = 0;
							 self->sprite = self->sprite_list.attack1;
	}break;
	case(ES_Dying) : {
						 self->sprite = self->sprite_list.dying;
	}break;
	}


}
void update_rollerenemy_ent(Entity *self){
	self->frame += 0.1;
	//self->think(self);
	int dir = 1;
	switch (self->dir){
	case(Right) :
	{
					self->flip = vector2d(0, 0);
					dir = 1;
	}break;
	case(Left) :
	{
				   self->flip = vector2d(1, 0);
				   dir = -1;
	}break;
	}
	if (!self->is_grounded){
		self->position.y += 1.5;  //might change to gravity
	}
	if (self->actionFrame > 0){
		self->actionFrame--;
	}
	if (self->in_attack){
		if ((!self->l_wall_collision&&self->dir!=Right)||(!self->r_wall_collision&&self->dir!=Left)){
			rollerenemy_displacement(self, vector2d(dir*self->movementspeed, 0));
		}
	}
	//end frames
	if (self->frame > self->sprite->frames_per_line - 1){
		if (self->actionFrame <= 0){
			if (self->in_attack){
				self->can_attack = true;
				self->in_action = false;
				self->in_attack = false;
				self->state = ES_Idle;
			}
			else{
				self->frame = 0;
				self->in_action = false;
			}
		}
		else{
			self->frame = 0;
		}
	}

}
void init_rollerenemy_ent(Entity *self, int ctr){
	self->state = ES_Idle;
	self->controlling = ctr;
	self->frame = 0;
	self->position = vector2d(100, 500);
	self->start_position = self->position;
	self->color = vector4d(255, 255, 255, 200);
	self->sprite_list.idle = gf2d_sprite_load_all("../images/test/enemy/shooter/shooter_idle.png", 32, 32, 3);
	self->sprite_list.dying = gf2d_sprite_load_all("../images/test/enemy/shooter/shooter_dying.png", 32, 32, 1);
	self->sprite_list.attack1 = gf2d_sprite_load_all("../images/test/enemy/shooter/shooter_shoot.png", 32, 32, 3);
	self->think = rollerenemy_think;
	self->movementspeed = 1;
	self->dashspeed = 3;
	self->maxjump = 1;
	self->actionFrame = 0;
	self->update_sprite = update_rollerenemy_sprite;
	self->is_grounded = false;
	self->can_attack = true;
	self->attack_trigger = false;
	self->in_action = false;
	self->l_wall_collision = false;
	self->r_wall_collision = false;
	self->attacknum = 0;
	self->dir = Right;
	self->type = ES_Enemy;
	self->attack = rollerenemy_attack;
	set_hitbox(self, self->position.x, self->position.y, 32, 32, 0, 0);
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