#include "g_crashman.h"
#include "simple_logger.h"
#include "g_projectile.h"
#include "g_game.h"
#include <math.h>

bool jump_across_m = false;
int dir_c = 0;
void crashman_think(Entity *self){
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
				if (self->is_grounded){
					if (target->in_attack == true){
						jump_across_m = true;
						if (self->action != jump)
							self->in_action = false;
						self->action = jump;
					}
					else{
						self->action = none;
					}
				}
					
				 if (self->can_attack==true){
						self->action = attack;
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
							  if (jump_across_m == true)
								  self->jumpFrame = 50;
							  if (target->position.x < self->position.x){
								  dir_c = -1;
							  }
							  else if (target->position.x > self->position.x){
								  dir_c = 1;
							  }
							  else{
								  dir_c = 0;
							  }
							  flip(self, target->position);
						  }
						  
							crashman_jump(self);
						  //self->actionFrame = 120;
						  

			}
				break;
			case none:{
						  self->in_action = true;
						  self->state = ES_Running;
						  self->update_sprite(self);
			}
				break;
			default:
				break;
			}
		}
	}
}
void crashman_attack(Entity *self){
	switch (self->attacknum){
	case(0) :
		{
			//slog("crashman attack 1");
			self->can_attack = false;
			self->in_action = true;
			self->in_attack = true;
			create_crashman_projectile(self, 3, self->attackdmg, 0);
			self->state = ES_Attacking;
			self->update_sprite(self);
		}
		break;
	default:
		//nothing?
		break;
	}
}

void crashman_jump(Entity *self){
	if (self->jumpFrame > 0){
		self->jumped = true;
		self->is_grounded = false;
		self->can_attack = false;
		crashman_displacement(self,vector2d(0,-3));
		self->jumpFrame--;		
		self->state = ES_Jump;
		self->update_sprite(self);
		//self->in_action = true;
	}
	else if(self->jumpFrame==0){
		//jump_across_m = false;
		self->jumped = false;
		self->can_attack = true;
	}
}
void update_crashman_sprite(Entity *self){
	switch (self->state){
		
	case(ES_Idle) : {
		//slog("idle time");	
			self->frame = 0;
			self->sprite = self->sprite_list.idle;
	}break;
	case(ES_Running) : {
						//slog("idle time");	
						self->frame = 0;
						self->sprite = self->sprite_list.run;
	}break;
	case(ES_Jump) : {
						//slog("jump shooter time");
						self->frame = 0;
						self->sprite = self->sprite_list.jump;
	}break;
	case(ES_Attacking) : {
							 //slog("crashman attack time");
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
void update_crashman_ent(Entity *self){
	self->frame += 0.1;
	//slog("State:%i", self->state);
	if (self->did_intro==false){
		//slog("Intro state");
		if (!self->health_created){
			flip(self, get_player_entity()->position);
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
					if (self->is_grounded){
						dir_c = 1;
					}
	}break;
	case(Left) :
	{
				   self->flip = vector2d(1, 0);
				   if (self->is_grounded){
					   dir_c = -1;
				   }
	}break;
	}
	if (self->did_intro){
		if (self->l_wall_collision){
			if (!self->is_grounded&&dir_c < 0){
				dir_c = 0;
			}
			else{
				dir_c = 1;
				self->dir = Right;
			}
		}

		if (self->r_wall_collision){
			if (!self->is_grounded&&dir_c > 0){
				dir_c = 0;
			}
			else{
				dir_c = -1;
				self->dir = Left;
			}
		}
		if (self->state!=ES_Dead)
			crashman_displacement(self, vector2d(dir_c, 0));
	}
	if (!self->is_grounded){
		self->position.y += 1.5;  //might change to gravity
	}
	if (self->is_grounded){
		self->jumped = false;
		self->can_attack = false;
		jump_across_m = false;
		self->jumpFrame = -1;
	}

	if (self->actionFrame > 0){
		self->actionFrame--;
	}
	//end frames
	if (self->frame >= self->sprite->frames_per_line - 1){
		if (self->actionFrame <= 0){
			if (self->in_attack){
				self->can_attack = false;
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
				set_input_control(1);
			}
			else{
				self->frame = 0;
				//self->in_action = false;
			}
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
void init_crashman_ent(Entity *self, int ctr){
	self->state = ES_Idle;
	self->controlling = ctr;
	self->frame = 0;
	self->position = vector2d(100, 600);
	self->start_position = self->position;
	self->color = vector4d(255, 255, 255, 255);
	self->sprite_list.idle = gf2d_sprite_load_all("../images/test/enemy/crashman/crashman_idle.png", 32, 32, 1);
	self->sprite_list.intro = gf2d_sprite_load_all("../images/test/enemy/crashman/crashman_intro.png", 32, 32, 3);
	self->sprite_list.dying = gf2d_sprite_load_all("../images/test/effect/boom.png", 56, 56, 15);
	//self->sprite_list.dying->sprite_offsety = 18;
	self->sprite_list.attack1 = gf2d_sprite_load_all("../images/test/enemy/crashman/crashman_attack1.png", 32, 32, 2);
	self->sprite_list.jump = gf2d_sprite_load_all("../images/test/enemy/crashman/crashman_jump.png", 32, 32, 1);
	self->sprite_list.run = gf2d_sprite_load_all("../images/test/enemy/crashman/crashman_run.png", 32, 32, 3);
	self->think = crashman_think;
	self->movementspeed = 1;
	self->dashspeed = 3;
	self->maxjump = 1;
	self->update_sprite = update_crashman_sprite;
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
	self->attack = crashman_attack;
	set_hitbox(self, self->position.x, self->position.y, 32, 32, 0, 0);
	self->action = none;
	self->update_ent = update_crashman_ent;
	self->sprite = self->sprite_list.intro;
	self->damage = crashman_damage;
	self->actionFrame = 240;
	self->health = 27;
	self->healthmax = 27;
	self->onDeath = crashman_death;
	self->attackdmg = 2;
	self->invincibleFrame = 0;
	self->reset = crashman_reset;
	self->clip = 1;
}
void crashman_set_position(Entity *self, Vector2D position){
	self->position = position;
}
void crashman_displacement(Entity *self, Vector2D position){
	self->position.x += position.x;
	self->position.y += position.y;
}
void crashman_damage(Entity *self, int damage, Vector2D kick){
	self->health -= damage;
	self->damageFrame = 60;
	self->invincibleFrame = 30;
}
void create_crashman_projectile(Entity *self, float speed, float dmg, int type){
	Entity *projectile = gf2d_entity_new();
	Projectiles proj;
	projectile->attackdmg = dmg;
	proj.parentType = ES_Boss;
	proj.destroyOnCollision = true;
	proj.destroyOnSurface = true;
	proj.aliveFrame = -1;
	proj.gravity = 0;
	proj.destroyeffect = 1;
	proj.stick = true;
	init_projectile_ent(projectile,360);
	int dir = (self->dir == Right) ? 1 : -1;
	Vector2D vec = vector2d(get_player_entity()->position.x - self->position.x, get_player_entity()->position.y - self->position.y);
	vector2d_normalize(&vec);
	vec.x *= 1.5;
	vec.y *= 1.5;
	switch (type){
	case 0:{
			
			   play_soundeffect("../sounds/lemon.wav", 0);
			   projectile->sprite = gf2d_sprite_load_all("../images/test/projectile/crashbomb.png", 13, 13, 3);
			   //slog("set");
			   projectile->position.x = (dir == 1) ? self->position.x + self->hitbox.offsetx + self->hitbox.w : self->position.x + self->hitbox.offsetx - 3;
			   projectile->position.y = self->position.y + self->hitbox.offsety + 7;
			   projectile->flip = self->flip;
			   proj.direction = vec;
			   set_hitbox(projectile, projectile->position.x, projectile->position.y, 13, 10, 0, 0);			  
			   projectile->clip = 1;
			   proj.heightTime = -1;
			   projectile->proj_data = proj;
			   //slog("x speed?:%f", ((get_player_entity()->position.x - projectile->position.x) / 180));
	}break;
	}
}

void crashman_death(Entity* self){
	set_input_control(0);
	Entity *player = get_player_entity();
	player->getPowerUp(player, 3);
	player->invincibleFrame = 9999;
	player->heldFrame = 0;
	player->damageFrame = 0;
	clear_key();
	stop_bgm();
	play_soundeffect("../sounds/dead.wav", 0);
	update_game_data("bossdata", 3, 1);
	update_game_data("weapondata", 3, 1);
	save_game();
}

void crashman_reset(Entity *self){
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