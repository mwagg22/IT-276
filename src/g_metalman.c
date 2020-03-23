#include "g_metalman.h"
#include "simple_logger.h"
#include "g_projectile.h"
#include "g_game.h"
#include <math.h>

bool jump_across = false;
int dir_m = 0;
void metalman_think(Entity *self){
	if (self->state != ES_Dead){
		float attack_range = 300.0;
		Entity *target = get_player_entity();
		////slog("think");
		if (target){
			if (vector2d_magnitude_between(target->position, self->position) > attack_range){
				self->action = none;
			}
			else{
				//slog("in range");
				if ((vector2d_magnitude_between(target->position, self->position) < attack_range / 4)){
					if (!self->in_action&&self->is_grounded)
						jump_across = true;
				}
					
				if (self->can_attack==true){
						slog("attacking");
						self->action = attack;
					}
					else{
						self->action = jump;
						slog("jumping");
					}
				}

		}
		if (self->in_action == false){

			switch (self->action){
			
			case attack:
				if (self->can_attack == true){
					flip(self, target->position);
					self->attack(self);
					self->actionFrame = 45;
				}
				break;
			case movement:
				break;
			case jump:{
						  if (self->jumped == false && (self->is_grounded == true)){
							  int rdm = (rand() % 3);
							  if (rdm < 1)
								  self->jumpFrame = 120;
							  else if (rdm < 2)
								  self->jumpFrame = 100;
							  else
								  self->jumpFrame = 60;
							  if (jump_across == true)
								  self->jumpFrame = 70;
						  }
							 
						  metalman_jump(self);
						  //self->actionFrame = 120;
						  

			}
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
void metalman_attack(Entity *self){
	switch (self->attacknum){
	case(0) :
		{
			//slog("metalman attack 1");
			self->can_attack = false;
			self->in_action = true;
			self->in_attack = true;
			create_metalman_projectile(self, 3, self->attackdmg, 0);
			self->state = ES_Attacking;
			self->update_sprite(self);
		}
		break;
	default:
		//nothing?
		break;
	}
}

void metalman_jump(Entity *self){
	if (self->jumpFrame > 0){
		self->jumped = true;
		self->is_grounded = false;
		self->can_attack = false;
		metalman_displacement(self,vector2d(0,-3));
		self->jumpFrame--;		
		self->state = ES_Jump;
		self->update_sprite(self);
		//self->in_action = true;
	}
	else{
		//jump_across = false;
		self->jumped = false;
		self->can_attack = true;
	}
}
void update_metalman_sprite(Entity *self){
	switch (self->state){
		
	case(ES_Idle) : {
		//slog("idle time");	
			self->frame = 0;
			self->sprite = self->sprite_list.idle;
	}break;
	case(ES_Jump) : {
						//slog("jump shooter time");
						self->frame = 0;
						self->sprite = self->sprite_list.jump;
	}break;
	case(ES_Attacking) : {
							 //slog("metalman attack time");
							 self->frame = 0;
							 if (self->attacknum == 0){
								 self->sprite = self->sprite_list.attack1;
								 play_soundeffect("../sounds/fire.wav", 0);
							 }
	}break;
	case(ES_Dead) : {
			self->sprite = self->sprite_list.dying;
	}break;
	}


}
void update_metalman_ent(Entity *self){
	self->frame += 0.1;
	//slog("State:%i", self->state);
	if (self->did_intro==false){
		//slog("Intro state");
		if (!self->health_created){
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
	if (self->clip != 0){
		if (self->position.y < get_game_camera()->topLeftBounds.y)
			self->position.y = get_game_camera()->topLeftBounds.y;
		if (self->position.x<get_game_camera()->topLeftBounds.x)
			self->position.x = get_game_camera()->topLeftBounds.x;
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
	if (jump_across==true){
		if (self->is_grounded==true)
			dir_m = (self->position.x > get_game_camera()->position.x + 150) ? -1 : 1;
		if (self->is_grounded == false)
			metalman_displacement(self, vector2d(dir_m, 0));
		else{
			slog("grownded");
		}
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
	if (self->is_grounded){
		self->jumped = false;
		self->can_attack = false;
		jump_across = false;
		self->jumpFrame = 0;
	}

	if (self->actionFrame > 0){
		self->actionFrame--;
	}
	//end frames
	if (self->frame >= self->sprite->frames_per_line - 1){
		if (self->actionFrame <= 0){
			if (self->in_attack){
				//self->can_attack = true;
				self->in_action = false;
				self->in_attack = false;
				self->state = ES_Idle;
				self->attacknum = 0;
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
				flip(self, get_player_entity()->position);
				set_input_control(1);
			}
			//else{
				//self->frame = 0;
				//self->in_action = false;
			//}
		}
		else{
			 if (self->in_attack){
				 self->frame = self->sprite->frames_per_line - 1;
			}
			 else{
				self->frame = self->sprite->frames_per_line - 1;
			}
		}
	}

}
void init_metalman_ent(Entity *self, int ctr){
	self->state = ES_Idle;
	self->controlling = ctr;
	self->frame = 0;
	self->position = vector2d(100, 600);
	self->start_position = self->position;
	self->color = vector4d(255, 255, 255, 255);
	self->sprite_list.idle = gf2d_sprite_load_all("../images/test/enemy/metalman/metalman_idle.png", 26, 36, 1);
	self->sprite_list.intro = gf2d_sprite_load_all("../images/test/enemy/metalman/metalman_intro.png", 26, 36, 2);
	self->sprite_list.dying = gf2d_sprite_load_all("../images/test/effect/boom.png", 56, 56, 15);
	//self->sprite_list.dying->sprite_offsety = 18;
	self->sprite_list.attack1 = gf2d_sprite_load_all("../images/test/enemy/metalman/metalman_attack1.png", 26, 36, 2);
	self->sprite_list.jump = gf2d_sprite_load_all("../images/test/enemy/metalman/metalman_jump.png", 26, 36, 1);
	//self->sprite_list.attack1->sprite_offsetx = 1;
	//self->sprite_list.attack1->sprite_offsety = 2;
	self->think = metalman_think;
	self->movementspeed = 1;
	self->dashspeed = 3;
	self->maxjump = 1;
	self->update_sprite = update_metalman_sprite;
	self->is_grounded = true;
	self->can_attack = true;
	self->attack_trigger = false;
	self->in_action = false;
	self->in_attack = false;
	self->did_intro = false;
	self->health_created = false;
	self->can_attack = false;
	self->jumped = false;
	self->type = ES_Boss;
	self->jumpFrame = 0;
	self->attacknum = 0;
	self->dir = Right;
	self->attack = metalman_attack;
	set_hitbox(self, self->position.x, self->position.y, 24, 24, 2, 5);
	self->action = none;
	self->update_ent = update_metalman_ent;
	self->sprite = self->sprite_list.intro;
	self->damage = metalman_damage;
	self->actionFrame = 240;
	self->health = 27;
	self->healthmax = 27;
	self->onDeath = metalman_death;
	self->attackdmg = 2;
	self->invincibleFrame = 0;
	self->reset = metalman_reset;
	self->clip = 1;
}
void metalman_set_position(Entity *self, Vector2D position){
	self->position = position;
}
void metalman_displacement(Entity *self, Vector2D position){
	self->position.x += position.x;
	self->position.y += position.y;
}
void metalman_damage(Entity *self, int damage, Vector2D kick){
	self->health -= damage;
	self->damageFrame = 60;
	self->invincibleFrame = 30;
}
void create_metalman_projectile(Entity *self, float speed, float dmg, int type){
	Entity *projectile = gf2d_entity_new();
	Projectiles proj;
	projectile->attackdmg = dmg;
	proj.parentType = ES_Boss;
	proj.destroyOnCollision = true;
	proj.aliveFrame = -1;
	init_projectile_ent(projectile,360);
	int dir = (self->dir == Right) ? 1 : -1;
	Vector2D vec = vector2d(get_player_entity()->position.x - self->position.x, get_player_entity()->position.y - self->position.y);
	vector2d_normalize(&vec);
	vec.x *= 2;
	vec.y *= 2;
	switch (type){
	case 0:{
			
			   play_soundeffect("../sounds/lemon.wav", 0);
			   projectile->sprite = gf2d_sprite_load_all("../images/test/projectile/metalblade_weapon.png", 16, 16, 3);
			   //slog("set");
			   projectile->position.x = (dir == 1) ? self->position.x + self->hitbox.offsetx + self->hitbox.w : self->position.x + self->hitbox.offsetx - 3;
			   projectile->position.y = self->position.y + self->hitbox.offsety + 7;
			   proj.direction = vec;
			   set_hitbox(projectile, projectile->position.x, projectile->position.y, 8, 5, 0, 0);			  
			   projectile->clip = 0;
			   proj.heightTime = -1;
			   projectile->proj_data = proj;
			   //slog("x speed?:%f", ((get_player_entity()->position.x - projectile->position.x) / 180));
	}break;
	}
}

void metalman_death(Entity* self){
	set_input_control(0);
	Entity *player = get_player_entity();
	player->getPowerUp(player, 2);
	player->invincibleFrame = 9999;
	player->heldFrame = 0;
	player->damageFrame = 0;
	clear_key();
	stop_bgm();
	play_soundeffect("../sounds/dead.wav", 0);
	update_game_data("bossdata", 2, 1);
	update_game_data("weapondata", 2, 1);
	save_game();
}

void metalman_reset(Entity *self){
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