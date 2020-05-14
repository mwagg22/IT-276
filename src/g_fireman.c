#include "g_fireman.h"
#include "simple_logger.h"
#include "g_projectile.h"
#include "g_game.h"
#include <math.h>
void fireman_think(Entity *self){
	if (self->state != ES_Dead){
		float attack_range = 300.0;
		Entity *target = get_player_entity();
		////slog("think");
		if (target){
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
					if (self->attacknum == 1){
						self->actionFrame = (vector2d_magnitude_between(target->position, self->position)) / self->movementspeed;
						self->invincibleFrame = self->actionFrame;
					}
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
			//slog("fireman attack 1");
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
			//slog("fireman attack 2");
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
		//slog("idle time");	
			self->frame = 0;
			self->sprite = self->sprite_list.idle;
	}break;
	case(ES_Jump) : {
						//slog("jump shooter time");
						self->frame = 0;
						self->sprite = self->sprite_list.idle;
	}break;
	case(ES_Attacking) : {
							 //slog("fireman attack time");
							 self->frame = 0;
							 if (self->attacknum==0)
								self->sprite = self->sprite_list.attack1;
							 else  if (self->attacknum == 1){
								 self->sprite = self->sprite_list.attack2;
								 play_soundeffect("../sounds/fire.wav", 0);
							 }
	}break;
	case(ES_Dead) : {
			self->sprite = self->sprite_list.dying;
	}break;
	}


}
void update_fireman_ent(Entity *self){
	self->frame += 0.1;
	//slog("State:%i", self->state);
	if (self->did_intro==false){
		//slog("Intro state");
		if (!self->health_created){
			gungirl_reset(get_player_entity());
			self->health = 1;			
			create_bar_ent(ES_Bossbar);
			self->health_created = true;
			play_bgm("../sounds/bosstheme.wav", 0);
		}
		set_input_control(0);
		if (self->health < self->healthmax){
			self->health += 1;
			play_soundeffect("../sounds/healthup.wav", 0);
		}
		
	}
	if (self->health >= self->healthmax){
		self->health = self->healthmax;
	}
	//slog("action frame:%i", self->actionFrame);
	if (self->health <= 0){
		////slog("im ded");
		if (self->state != ES_Dead){
			self->state = ES_Dead;
			//slog("ded");
			self->update_sprite(self);
			self->onDeath(self);
			self->in_attack = false;
			gungirl_reset(get_player_entity());
			set_input_control(0);
			self->color = vector4d(255, 255, 255, 255);
			
		}
		else{
			if (self->frame > self->sprite->frames_per_line - 1){
				if (self->state == ES_Dead){
					//slog("dead");
					//update_boss_state();
					set_input_control(0);
					self->color = vector4d(255, 255, 255, 0);
				}
			}
		}
		//return;
	}
	if (self->did_intro){
	if (self->state != ES_Hit || self->state != ES_Dying || self->state != ES_Dead)
		self->think(self);
	}
	if (self->damageFrame > 0){
		self->damageFrame--;
		if (self->damageFrame % 3 == 1)
			self->color = vector4d(255, 255, 255, 0);
		else
			self->color = vector4d(255, 255, 255, 255);
	}
	if (self->invincibleFrame > 0){
		self->invincibleFrame--;
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
				if (self->frame > 4.0 &&self->frame <4.2)
					play_soundeffect("../sounds/fire.wav", 0);
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
	if (self->frame >= self->sprite->frames_per_line - 1){
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
			else if (self->state == ES_Dead){
				//slog("dead");
				update_boss_state();
				self->frame = self->sprite->frames_per_line - 1;
				
			}
			else if (!self->did_intro){
				//slog("intro done");
				self->frame = self->sprite->frames_per_line - 1;				
				self->state = ES_Idle;
				self->did_intro = true;
				set_input_control(1);
			}
			else{
				self->frame = 0;
				self->in_action = false;
			}
		}
		else{
			 if (self->in_attack){
				self->frame = 7;
			}
			 else{
				self->frame = self->sprite->frames_per_line - 1;
			}
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
	self->sprite_list.idle = gf2d_sprite_load_all("../images/test/enemy/fireman/fireman_idle.png", 32, 32, 2);
	self->sprite_list.intro = gf2d_sprite_load_all("../images/test/enemy/fireman/fireman_intro.png", 32, 32, 3);
	self->sprite_list.dying = gf2d_sprite_load_all("../images/test/effect/boom.png", 56, 56, 15);
	self->sprite_list.dying->sprite_offsety = 18;
	self->sprite_list.attack1 = gf2d_sprite_load_all("../images/test/enemy/fireman/fireman_attack1.png", 32, 32, 2);
	self->sprite_list.attack2 = gf2d_sprite_load_all("../images/test/enemy/fireman/fireman_attack2.png", 32, 32, 10);
	self->think = fireman_think;
	self->movementspeed = 1;
	self->dashspeed = 3;
	self->maxjump = 1;
	self->update_sprite = update_fireman_sprite;
	self->is_grounded = false;
	self->can_attack = true;
	self->attack_trigger = false;
	self->in_action = false;
	self->in_attack = false;
	self->did_intro = false;
	self->health_created = false;
	self->type = ES_Boss;
	self->attacknum = 0;
	self->dir = Right;
	self->attack = fireman_attack;
	set_hitbox(self, self->position.x, self->position.y, 32, 32, 0, 0);
	self->action = none;
	self->update_ent = update_fireman_ent;
	self->sprite = self->sprite_list.intro;
	self->damage = fireman_damage;
	self->actionFrame = 240;
	self->health = 27;
	self->healthmax = 27;
	self->onDeath = fireman_death;
	self->attackdmg = 2;
	self->invincibleFrame = 0;
	self->reset = fireman_reset;
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
	self->invincibleFrame = 30;
}
void create_fireman_projectile(Entity *self, float speed, float dmg, int type){
	Entity *projectile = gf2d_entity_new();
	Entity *projectile2 = gf2d_entity_new();
	Entity *projectile3 = gf2d_entity_new();
	Projectiles proj;
	projectile->attackdmg = dmg;
	projectile2->attackdmg = dmg;
	projectile3->attackdmg = dmg;
	proj.parentType = ES_Boss;
	proj.destroyOnCollision = true;
	proj.destroyOnSurface = false;
	proj.destroyeffect = -1;
	proj.aliveFrame = -1;
	proj.gravity = true;
	proj.stick = 0;
	init_projectile_ent(projectile,360);
	init_projectile_ent(projectile2, 360);
	init_projectile_ent(projectile3, 360);
	int dir = (self->dir == Right) ? 1 : -1;
	switch (type){
	case 0:{
			
			   play_soundeffect("../sounds/lemon.wav", 0);
			   projectile->sprite = gf2d_sprite_load_all("../images/test/projectile/fire.png", 8, 8, 3);
			   //slog("set");
			   projectile->position.x = (dir == 1) ? self->position.x + self->hitbox.offsetx + self->hitbox.w : self->position.x + self->hitbox.offsetx - 3;
			   projectile->position.y = self->position.y + self->hitbox.offsety + 7;
			   proj.direction = vector2d(dir*fabs((get_player_entity()->position.x - projectile->position.x) / 180), -2.5);
			   set_hitbox(projectile, projectile->position.x, projectile->position.y, 8, 5, 0, 0);			  
			   projectile->clip = 1;
			   proj.heightTime = 60;
			   projectile->proj_data = proj;
			   //slog("x speed?:%f", ((get_player_entity()->position.x - projectile->position.x) / 180));
			  
			   projectile2->sprite = gf2d_sprite_load_all("../images/test/projectile/fire.png", 8, 8, 3);
			   //slog("set");
			   projectile2->position.x = (dir == 1) ? self->position.x + self->hitbox.offsetx + self->hitbox.w : self->position.x + self->hitbox.offsetx - 3;
			   projectile2->position.y = self->position.y + self->hitbox.offsety + 7;
			   set_hitbox(projectile2, projectile2->position.x, projectile2->position.y, 8, 5, 0, 0);		
			   proj.direction = vector2d(dir*fabs((get_player_entity()->position.x - projectile2->position.x) / 180), -1.8);
			   projectile2->clip = 1;
			   proj.heightTime = 60;
			   projectile2->proj_data = proj;

			   
			   projectile3->sprite = gf2d_sprite_load_all("../images/test/projectile/fire.png", 8, 8, 3);
			   //slog("set");
			   projectile3->position.x = (dir == 1) ? self->position.x + self->hitbox.offsetx + self->hitbox.w : self->position.x + self->hitbox.offsetx - 3;
			   projectile3->position.y = self->position.y + self->hitbox.offsety + 7;
			   proj.direction = vector2d(dir*fabs((get_player_entity()->position.x - projectile3->position.x) / 180), -.8);
			   set_hitbox(projectile3, projectile3->position.x, projectile3->position.y, 8, 5, 0, 0);		   
			   projectile3->clip = 1;
			   proj.heightTime = 60;
			   projectile3->proj_data = proj;
			   //slog("height time :%i", proj.heightTime);
	}break;
	}
}

void fireman_death(Entity* self){
	set_input_control(0);
	Entity *player = get_player_entity();
	player->getPowerUp(player, 1);
	player->invincibleFrame = 9999;
	player->heldFrame = 0;
	player->damageFrame = 0;
	clear_key();
	stop_bgm();
	play_soundeffect("../sounds/dead.wav", 0);
	update_game_data("bossdata", 0, 1);
	update_game_data("weapondata", 1, 1);
	save_game();
}

void fireman_reset(Entity *self){
	self->state = ES_Idle;
	self->sprite = self->sprite_list.attack1;
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
	self->type = ES_Boss;
	self->attacknum = 0;
	self->dir = Right;
	self->action = none;
	self->actionFrame = 240;
	self->health = 27;
	self->healthmax = 27;
	self->attackdmg = 2;
	self->invincibleFrame = 0;
	self->damageFrame = 0;
}