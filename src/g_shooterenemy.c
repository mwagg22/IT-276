#include "g_shooterenemy.h"
#include "simple_logger.h"
#include "g_effects.h"
#include "g_item.h"
#include "g_projectile.h"
#include <math.h>
void shooterenemy_think(Entity *self){
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
				////slog("in range");
				int rdm = (rand() % 11);
				if (rdm < 1)
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
						  flip(self, target->position);
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
void shooterenemy_attack(Entity *self){
	switch (self->attacknum){
	case(0) :
		if (self->is_grounded == true){
			////slog("shooter attack");
			self->can_attack = false;
			self->in_action = true;
			self->in_attack = true;
			create_shooterenemy_projectile(self, 3, self->attackdmg, 0);
			self->state = ES_Attacking;
			self->update_sprite(self);
		}
		break;
	default:
		//nothing?
		break;
	}
}
void update_shooterenemy_sprite(Entity *self){
	switch (self->state){
		
	case(ES_Idle) : {
		////slog("idle time");
		
			self->frame = 0;
			self->sprite = self->sprite_list.idle;
	}break;
	case(ES_Jump) : {
						//slog("jump shooter time");
						self->frame = 0;
						self->sprite = self->sprite_list.idle;
	}break;
	case(ES_Attacking) : {
							 //slog("shooter time");
							 self->frame = 0;
							 self->sprite = self->sprite_list.attack1;
	}break;
	case(ES_Dying) : {
			self->sprite = self->sprite_list.dying;
	}break;
	}


}
void update_shooterenemy_ent(Entity *self){
	self->frame += 0.1;
	if (self->state!=ES_Hit||self->state!=ES_Dead)
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

	//end frames
	if (self->frame >= self->sprite->frames_per_line - 1){
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

}
void init_shooterenemy_ent(Entity *self, int ctr){
	self->state = ES_Idle;
	self->controlling = ctr;
	self->frame = 0;
	self->position = vector2d(100, 500);
	self->start_position = self->position;
	self->color = vector4d(255, 255, 255, 255);
	self->sprite_list.idle = gf2d_sprite_load_all("../images/test/enemy/shooter/shooter_idle.png", 32, 32, 2);
	self->sprite_list.dying = gf2d_sprite_load_all("../images/test/enemy/shooter/shooter_dying.png", 32, 32, 1);
	self->sprite_list.attack1 = gf2d_sprite_load_all("../images/test/enemy/shooter/shooter_shoot.png", 32, 32, 3);
	self->think = shooterenemy_think;
	self->movementspeed = 1;
	self->dashspeed = 3;
	self->maxjump = 1;
	self->update_sprite = update_shooterenemy_sprite;
	self->is_grounded = false;
	self->can_attack = true;
	self->attack_trigger = false;
	self->in_action = false;
	self->attacknum = 0;
	self->dir = Right;
	self->attack = shooterenemy_attack;
	set_hitbox(self, self->position.x, self->position.y, 32, 32, 0, 0);
	self->action = none;
	self->update_ent = update_shooterenemy_ent;
	self->sprite = self->sprite_list.idle;
	self->damage = shooterenemy_damage;
	self->type = ES_Enemy;
	self->onDeath = shooterenemy_death;
	self->health = 10;
	self->healthmax = 10;
	self->attackdmg = 5;
	self->reset = shooterenemy_reset;
}
void shooterenemy_set_position(Entity *self, Vector2D position){
	self->position = position;
}
void shooterenemy_displacement(Entity *self, Vector2D position){
	self->position.x += position.x;
	self->position.y += position.y;
}
void shooterenemy_damage(Entity *self, int damage, Vector2D kick){
	self->health -= damage;
	self->damageFrame = 60;
}
void shooterenemy_death(Entity* self){
	create_effect(vector2d(self->position.x + self->hitbox.w / 2 - 5, self->position.y), 0, -1);
	int rdm = (rand() % 11);
	if (rdm < 2)
		create_item(I_Energy, 6, 300, vector2d(self->position.x+self->hitbox.w/2, self->position.y));
	else if (rdm>2&&rdm<5)
		create_item(I_Health, 6, 300, vector2d(self->position.x + self->hitbox.w / 2, self->position.y));
	else if (rdm>5 && rdm<9)
		create_item(I_Bolt, 6, 300, vector2d(self->position.x + self->hitbox.w / 2, self->position.y));
}
void create_shooterenemy_projectile(Entity *self, float speed, float dmg, int type){
	Entity *projectile = gf2d_entity_new();
	Projectiles proj;
	projectile->attackdmg = dmg;
	proj.parentType = ES_Enemy;
	init_projectile_ent(projectile,-1);
	int dir = (self->dir == Right) ? 1 : -1;
	switch (type){
	case 0:{
			   proj.direction = vector2d(dir*speed * 1, 0);
			   proj.destroyOnCollision = true;
			   proj.aliveFrame = -1;
			   projectile->sprite = gf2d_sprite_load_all("../images/test/projectile/lemon.png", 8, 5, 1);
			   //slog("set");
			   projectile->position.x = (dir == 1) ? self->position.x + self->hitbox.offsetx + self->hitbox.w : self->position.x + self->hitbox.offsetx - 3;
			   projectile->position.y = self->position.y + self->hitbox.offsety + 7;
			   set_hitbox(projectile, projectile->position.x, projectile->position.y, 8, 5, 0, 0);
			   projectile->proj_data = proj;
	}break;
	}
}
void shooterenemy_reset(Entity *self){
	self->state = ES_Idle;
	self->frame = 0;
	self->color = vector4d(255, 255, 255, 255);
	self->movementspeed = 1;
	self->dashspeed = 3;
	self->maxjump = 1;
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