#include "g_lookerenemy.h"
#include "simple_logger.h"
#include "g_effects.h"
#include "g_item.h"
#include <math.h>
#include "g_effects.h"

void lookerenemy_think(Entity *self){
	if (self->state != ES_Dead){
		float attack_range = 200.0;
		Entity *target = get_player_entity();
		////slog("think");
		if (target->_inuse){
			if (vector2d_magnitude_between(target->position, self->position) > attack_range){
				self->action = none;
				////slog("not in range");
			}
			else{
					self->action = attack;
			}

		}
		if (self->in_action == false){

			switch (self->action){

			case attack:
				if (self->can_attack == true){
					flip(self, target->position);
					self->in_attack = true;
					self->in_action = true;
				}
				break;
			case movement:
				break;
			case none:{
						  //flip(self, target->position);
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
void lookerenemy_attack(Entity *self){
	switch (self->attacknum){
	case(0) :
		if (self->is_grounded == true){
			////slog("roller attack");
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
void update_lookerenemy_sprite(Entity *self){
	switch (self->state){

	case(ES_Idle) : {
						////slog("idle time");

						self->frame = 0;
						self->sprite = self->sprite_list.idle;
	}break;
	case(ES_Jump) : {
						////slog("jump roller time");
						self->frame = 0;
						self->sprite = self->sprite_list.idle;
	}break;
	case(ES_Attacking) : {
							 ////slog("roller time");
							 self->frame = 0;
							 self->sprite = self->sprite_list.attack1;
	}break;
	case(ES_Dying) : {
						 self->sprite = self->sprite_list.dying;
	}break;
	}


}
void update_lookerenemy_ent(Entity *self){
	self->frame += 0.1;
	if (self->state != ES_Hit || self->state != ES_Dead)
		self->think(self);
	if (self->health <= 0){
		//slog("im ded");
		if (self->state != ES_Dead){
			self->state = ES_Dead;
			self->onDeath(self);
			self->color = vector4d(255, 255, 255, 0);
			
		}
		return;
	}
	if (self->damageFrame > 0){
		self->damageFrame--;
		if (self->damageFrame % 3 == 1)
			self->color = vector4d(255, 255, 255, 0);
		else
			self->color = vector4d(255, 255, 255, 255);
	}
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
	if (self->actionFrame > 0){
		self->actionFrame--;
	}
	if (self->in_attack){
		int y_dir = (get_player_entity()->position.y > self->position.y) ? 1 : -1;
			lookerenemy_displacement(self, vector2d(dir*self->movementspeed, y_dir*self->movementspeed));
	}
	//end frames
	if (self->frame > self->sprite->frames_per_line - 1){
		if (self->actionFrame <= 0){
			if (self->in_attack){
				self->can_attack = true;
				self->in_action = false;
				self->in_attack = false;
				self->frame = 0;
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
void init_lookerenemy_ent(Entity *self, int ctr){
	self->state = ES_Idle;
	self->controlling = ctr;
	self->frame = 0;
	self->position = vector2d(100, 700);
	self->start_position = self->position;
	self->color = vector4d(255, 255, 255, 255);
	self->sprite_list.idle = gf2d_sprite_load_all("../images/test/enemy/looker/looker_idle.png", 16, 16, 5);
	self->sprite_list.dying = gf2d_sprite_load_all("../images/test/enemy/shooter/shooter_dying.png", 32, 32, 1);
	self->sprite_list.attack1 = gf2d_sprite_load_all("../images/test/enemy/looker/looker_idle.png", 16, 16, 5);
	self->think = lookerenemy_think;
	self->movementspeed = .5;
	self->dashspeed = 3;
	self->maxjump = 1;
	self->actionFrame = 0;
	self->update_sprite = update_lookerenemy_sprite;
	self->is_grounded = false;
	self->can_attack = true;
	self->attack_trigger = false;
	self->in_action = false;
	self->l_wall_collision = false;
	self->r_wall_collision = false;
	self->attacknum = 0;
	self->dir = Right;
	self->type = ES_Enemy;
	self->attack = lookerenemy_attack;
	set_hitbox(self, self->position.x, self->position.y, 16, 16, 0, 0);
	self->action = none;
	self->damage = lookerenemy_damage;
	self->update_ent = update_lookerenemy_ent;
	self->sprite = self->sprite_list.idle;
	self->onDeath = lookerenemy_death;
	self->health = 10;
	self->healthmax = 10;
	self->attackdmg = 5;
	self->reset = lookerenemy_reset;
}
void lookerenemy_set_position(Entity *self, Vector2D position){
	self->position = position;
}
void lookerenemy_displacement(Entity *self, Vector2D position){
	self->position.x += position.x;
	self->position.y += position.y;
}
void lookerenemy_damage(Entity *self, int damage, Vector2D kick){
	self->health -= damage;
	self->damageFrame = 60;
}
void lookerenemy_death(Entity* self){
	Effects data;
	data.centered = true;
	data.offset = vector2d(0, 0);
	data.type = 0;
	create_effect(vector2d(self->position.x + self->hitbox.w / 2 - 5, self->position.y), 0, -1,data);
	int rdm = (rand() % 11);
	if (rdm < 2)
		create_item(I_Energy, 6, 300, vector2d(self->position.x + self->hitbox.w / 2, self->position.y));
	else if (rdm>2 && rdm<5)
		create_item(I_Health, 6, 300, vector2d(self->position.x + self->hitbox.w / 2, self->position.y));
	else if (rdm>5 && rdm<9)
		create_item(I_Bolt, 6, 300, vector2d(self->position.x + self->hitbox.w / 2, self->position.y));
}

void lookerenemy_reset(Entity *self){
	self->state = ES_Idle;
	self->frame = 0;
	self->color = vector4d(255, 255, 255, 255);
	self->is_grounded = false;
	self->can_attack = true;
	self->attack_trigger = false;
	self->in_action = false;
	self->in_attack = false;
	self->did_intro = false;
	self->health_created = false;
	self->type = ES_Enemy;
	self->attacknum = 0;
	self->dir = Right;
	self->action = none;
	self->actionFrame = 0;
	self->health = 10;
	self->healthmax = 10;
	self->attackdmg = 5;
	self->invincibleFrame = 0;
	self->damageFrame = 0;
}