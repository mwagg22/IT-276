#include "g_fireman.h"
#include "simple_logger.h"
#include "g_projectile.h"
#include <math.h>
void fireman_think(Entity *self){
	if (self->state != ES_Dead){
		float attack_range = 200.0;
		Entity *target = get_player_entity();
		//slog("think");
		if (target->_inuse){
			if (vector2d_magnitude_between(target->position, self->position) > attack_range){
				self->action = none;
				//slog("not in range");
			}
			else{
				//slog("in range");
				if ((vector2d_magnitude_between(target->position, self->position) < attack_range/4))
					self->attacknum = 1;
				int rdm = (rand() % 11);
				if (rdm < 3)
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
					if (self->attacknum == 1)
						self->actionFrame = (vector2d_magnitude_between(target->position, self->position)) / self->movementspeed;
				}
				break;
			case movement:
				break;
			case none:{
						  flip(self, target->position);
						  self->in_action = true;
						  self->state = ES_Idle;
						  self->update_sprite(self);
						  self->actionFrame = (rand() % 60);
			}
				break;
			default:
				break;
			}
		}
	}
}
void fireman_attack(Entity *self){
	switch (self->attacknum){
	case(0) :
		if (self->is_grounded == true){
			slog("fireman attack 1");
			self->can_attack = false;
			self->in_action = true;
			self->in_attack = true;
			create_fireman_projectile(self, 3, self->attackdmg, 0);
			self->state = ES_Attacking;
			self->update_sprite(self);
		}
		break;
	case(1) :
		if (self->is_grounded == true){
			slog("fireman attack 2");
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
void update_fireman_sprite(Entity *self){
	switch (self->state){
		
	case(ES_Idle) : {
		slog("idle time");	
			self->frame = 0;
			self->sprite = self->sprite_list.idle;
	}break;
	case(ES_Jump) : {
						slog("jump shooter time");
						self->frame = 0;
						self->sprite = self->sprite_list.idle;
	}break;
	case(ES_Attacking) : {
							 slog("fireman attack time");
							 self->frame = 0;
							 if (self->attacknum==0)
								self->sprite = self->sprite_list.attack1;
							 else  if (self->attacknum == 1)
								 self->sprite = self->sprite_list.attack2;
	}break;
	case(ES_Dying) : {
			self->sprite = self->sprite_list.dying;
	}break;
	}


}
void update_fireman_ent(Entity *self){
	self->frame += 0.1;
	if (self->health <= 0){
		//slog("im ded");
		if (self->state != ES_Dead){
			self->state = ES_Dead;
			self->onDeath(self);
			self->color = vector4d(255, 255, 255, 0);
			
		}
		return;
	}
	if (self->state!=ES_Hit||self->state!=ES_Dying||self->state!=ES_Dead)
		self->think(self);
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
	if (self->in_attack){
		if (self->attacknum == 0){
			//set_hitbox(self, self->position.x, self->position.y, 29, 25, 0, 0);
		}
		if (self->attacknum == 1&&self->frame>4){
			if ((!self->l_wall_collision&&self->dir != Right) || (!self->r_wall_collision&&self->dir != Left)){
				if (self->dir == Right)
					self->position.x += 1;
				else
					self->position.x -= 1;
			}
		}
	}
	if (self->actionFrame > 0){
		self->actionFrame--;
	}
	//end frames
	if (self->frame > self->sprite->frames_per_line - 1){
		if (self->actionFrame <= 0){
			if (self->in_attack){
				self->can_attack = true;
				self->in_action = false;
				self->in_attack = false;
				self->state = ES_Idle;
				if (self->attacknum == 0){
					self->attacknum = 1;
				}
				else{
					self->attacknum = 0;
				}
			}
			else if (self->state == ES_Dying){
				self->frame = self->sprite->frames_per_line - 1;

			}
			else{
				self->frame = 0;
				self->in_action = false;
			}
		}
		else{
			if (self->in_attack)
				self->frame = 7;
			else
				self->frame = 0;
		}
	}

}
void init_fireman_ent(Entity *self, int ctr){
	self->state = ES_Idle;
	self->controlling = ctr;
	self->frame = 0;
	self->position = vector2d(100, 600);
	self->start_position = self->position;
	self->color = vector4d(255, 255, 255, 255);
	self->sprite_list.idle = gf2d_sprite_load_all("../images/test/enemy/fireman/fireman_idle.png", 29, 25, 1);
	self->sprite_list.dying = gf2d_sprite_load_all("../images/test/enemy/fireman/fireman_dying.png", 52, 48, 1);
	self->sprite_list.attack1 = gf2d_sprite_load_all("../images/test/enemy/fireman/fireman_attack1.png", 30, 28, 2);
	self->sprite_list.attack1->sprite_offsetx = 1;
	self->sprite_list.attack1->sprite_offsety = 2;
	self->sprite_list.attack2 = gf2d_sprite_load_all("../images/test/enemy/fireman/fireman_attack2.png", 52, 48, 10);
	self->sprite_list.attack2->sprite_offsetx = 20;
	self->sprite_list.attack2->sprite_offsety = 15;
	self->think = fireman_think;
	self->movementspeed = 1;
	self->dashspeed = 3;
	self->maxjump = 1;
	self->update_sprite = update_fireman_sprite;
	self->is_grounded = false;
	self->can_attack = true;
	self->attack_trigger = false;
	self->in_action = false;
	self->attacknum = 0;
	self->dir = Right;
	self->attack = fireman_attack;
	set_hitbox(self, self->position.x, self->position.y, 29, 25, 0, 0);
	self->action = none;
	self->update_ent = update_fireman_ent;
	self->sprite = self->sprite_list.idle;
	self->damage = fireman_damage;
	self->type = ES_Enemy;
	self->actionFrame = 0;
	self->health = 50;
	self->healthmax = 50;
	self->onDeath = fireman_death;
}
void fireman_set_position(Entity *self, Vector2D position){
	self->position = position;
}
void fireman_displacement(Entity *self, Vector2D position){
	self->position.x += position.x;
	self->position.y += position.y;
}
void fireman_damage(Entity *self, int damage, Vector2D kick){
	self->health -= damage;
	self->damageFrame = 60;
}
void create_fireman_projectile(Entity *self, float speed, float dmg, int type){
	Entity *projectile = gf2d_entity_new();
	Entity *projectile2 = gf2d_entity_new();
	Entity *projectile3 = gf2d_entity_new();
	Projectiles proj;
	projectile->attackdmg = dmg;
	proj.parentType = ES_Enemy;
	proj.destroyOnCollision = true;
	proj.aliveFrame = -1;
	init_projectile_ent(projectile,120);
	init_projectile_ent(projectile2, 120);
	init_projectile_ent(projectile3, 120);
	int dir = (self->dir == Right) ? 1 : -1;
	switch (type){
	case 0:{
			   proj.direction = vector2d(dir*speed * 1, -.7);
			   
			   projectile->sprite = gf2d_sprite_load_all("../images/test/projectile/fire.png", 8, 8, 3);
			   slog("set");
			   projectile->position.x = (dir == 1) ? self->position.x + self->hitbox.offsetx + self->hitbox.w : self->position.x + self->hitbox.offsetx - 3;
			   projectile->position.y = self->position.y + self->hitbox.offsety + 7;
			   set_hitbox(projectile, projectile->position.x, projectile->position.y, 8, 5, 0, 0);
			   projectile->proj_data = proj;
			   projectile->clip = 1;

			   proj.direction = vector2d(dir*speed * 1, -.6);
			   projectile2->sprite = gf2d_sprite_load_all("../images/test/projectile/fire.png", 8, 8, 3);
			   slog("set");
			   projectile2->position.x = (dir == 1) ? self->position.x + self->hitbox.offsetx + self->hitbox.w : self->position.x + self->hitbox.offsetx - 3;
			   projectile2->position.y = self->position.y + self->hitbox.offsety + 7;
			   set_hitbox(projectile2, projectile2->position.x, projectile2->position.y, 8, 5, 0, 0);
			   projectile2->proj_data = proj;
			   projectile2->clip = 1;

			   proj.direction = vector2d(dir*speed * 1, -.1);
			   projectile3->sprite = gf2d_sprite_load_all("../images/test/projectile/fire.png", 8, 8, 3);
			   slog("set");
			   projectile3->position.x = (dir == 1) ? self->position.x + self->hitbox.offsetx + self->hitbox.w : self->position.x + self->hitbox.offsetx - 3;
			   projectile3->position.y = self->position.y + self->hitbox.offsety + 7;
			   set_hitbox(projectile3, projectile3->position.x, projectile3->position.y, 8, 5, 0, 0);
			   projectile3->proj_data = proj;
			   projectile3->clip = 1;
			   
	}break;
	}
}

void fireman_death(Entity* self){
	Entity *player = get_player_entity();
	player->getPowerUp(player, 1);
}