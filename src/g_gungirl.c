#include "g_gungirl.h"
#include "simple_logger.h"
#include "g_projectile.h"
#include "g_game.h"
#include "g_music.h"
#include <math.h>
SDL_Event event;
float dx_a, dy_a = 0;
Actioninput prev_action_a = none;

void gungirl_think(Entity *self){
	switch (self->action){
	case attack:
		if (self->can_attack == true){
			gungirl_attack(self);
			prev_action_a = self->action;
		}
		break;
	case special:
		if (self->can_special == true){
			gungirl_special(self);
			prev_action_a = self->action;
		}
		break;
	case movement:
		if (self->action != prev_action_a || self->attack_trigger == true && self->can_attack == true || (self->is_grounded == true && self->sprite == self->sprite_list.idle || self->sprite == self->sprite_list.jump)){
			self->update_sprite(self);
			prev_action_a = self->action;
		}
		break;
	case dash:
	{
				 gungirl_dash(self);
				 self->update_sprite(self);
				 prev_action_a = self->action;
	}
		break;
	case jump:
		if (self->action != prev_action_a || self->jumpFrame > 0){
			gungirl_jump(self);
			self->update_sprite(self);
			prev_action_a = self->action;
		}
		break;
	default:
		if (self->action != prev_action_a || self->attack_trigger == true && self->can_attack == true){
			self->update_sprite(self);
			prev_action_a = self->action;
		}
		break;
	}

}
void gungirl_attack(Entity *self){
	if (self->weapons_list.currentWeapon == 0){
		switch (self->attacknum){
		case(0) :
		{
					create_gungirl_projectile(self, 3, self->attackdmg, self->attacknum);
		}
				//fire gun
				break;
		case(1) :
			//fire stronger gun?
		{
					create_gungirl_projectile(self, 3, self->attackdmg * 2, self->attacknum);
		}
				break;
		}
	}
	else{
		create_gungirl_projectile(self, 3, self->attackdmg * 2, self->weapons_list.currentWeapon + 1);
	}
}
void gungirl_special(Entity *self){
	switch (self->specialnum){
	case(0) :
		if (self->is_grounded == true){
			if (self->is_dashing == true){
				//dashing shoot
			}
			else{
				//grounded shoot
			}
		}
		else{
			//in air shoot
		}
		//fire gun
		break;
	case(1) :
		//slide?
		break;
	default:
		//nothing?
		break;
	}
}
void gungirl_dash(Entity *self){
	self->is_dashing = true;
	self->state = ES_Dash;
	if (self->dashFrame < 50){
		if (self->dir == Right){
			if (!self->r_wall_collision)
				gungirl_displacement(self, vector2d(self->dashspeed, 0));
			else{
				gungirl_displacement(self, vector2d(0, 0));
				self->dashFrame = 50;
			}
		}
		else{
			if (!self->l_wall_collision)
				gungirl_displacement(self, vector2d(-self->dashspeed, 0));
			else{
				gungirl_displacement(self, vector2d(0, 0));
				self->dashFrame = 50;
			}
		}
	}
	else{
		self->is_dashing = false;
		if (self->is_grounded == true){
			self->state = ES_Idle;
		}
		else{
			self->state = ES_Jump;
		}
	}
}
void gungirl_jump(Entity *self){
	if (self->jumpFrame < 20 && (self->maxjump>0 || self->wall_jumped || self->is_wall_sliding)){
		if (!self->is_wall_sliding){
			self->jumped = true;
			self->is_grounded = false;
			self->state = ES_Jump;
			gungirl_displacement(self, vector2d(0, -4));
		}
		else{
			self->wall_jumped = true;
			if (self->dir == Right){
				self->is_grounded = false;
				self->state = ES_Jump;
				gungirl_displacement(self, vector2d(-19, -3));
			}
			else{
				self->is_grounded = false;
				self->state = ES_Jump;
				gungirl_displacement(self, vector2d(19, -3));
			}
		}
	}
}
void gungirl_damage(Entity *self, int damage, Vector2D kick){
	self->health -= damage;
	self->state = ES_Hit;
	self->update_sprite(self);
	gungirl_displacement(self, kick);
	play_soundeffect("../sounds/damage.wav", 0);
}
void update_gungirl_sprite(Entity *self){
	switch (self->state){

	case(ES_Idle) : {//slog("idle time");
						if (self->in_attack == true){
							if (self->sprite != self->sprite_list.idleAttack){
								self->frame = 1;
								//slog("idle attack");
								self->sprite = self->sprite_list.idleAttack;
							}
							//self->frame=frame;
						}
						else if (self->attack_trigger == true && self->can_attack == true && self->heldFrame < 9){
							self->sprite = self->sprite_list.idleAttack;
							//run attack
							//slog("attacking");
							self->attack(self);
							self->in_attack = true;
							self->can_attack = false;
							self->frame = 0;
						}
						else{
							self->frame = 0;
							self->sprite = self->sprite_list.idle;
						}
	}break;
	case(ES_Running) : {
						   if (self->in_attack == true){
							   if (self->sprite != self->sprite_list.runAttack){
								   self->frame = 1;
								   self->sprite = self->sprite_list.runAttack;
								   //slog("run attack");
							   }
							   //self->frame=frame;
						   }
						   else if (self->attack_trigger == true && self->can_attack == true && self->heldFrame < 9){
							   self->frame = 0;
							   self->sprite = self->sprite_list.runAttack;
							   self->attack(self);
							   //run attack
							   //slog("attacking dos");
							   self->in_attack = true;
							   self->can_attack = false;
						   }

						   else{
							   //slog("running");
							   //self->frame = 0;
							   self->sprite = self->sprite_list.run;
						   }
	}break;
	case(ES_Attacking) : {

	}break;
	case(ES_Jump) : {
						if (self->in_attack){
							if (self->sprite != self->sprite_list.jumpAttack || self->sprite != self->sprite_list.wallAttack){
								if (!self->is_wall_sliding){
									self->sprite = self->sprite_list.jumpAttack;
									//slog("hi");
								}
								else
									self->sprite = self->sprite_list.wallAttack;
							}
							//self->frame=frame;
						}
						else if (self->attack_trigger == true && self->can_attack == true && self->heldFrame < 9){
							self->attack(self);
							if (!self->is_wall_sliding){
								self->frame = 0;
								self->sprite = self->sprite_list.jumpAttack;
								//run attack
								//slog("jump attack");
								self->in_attack = true;
								self->can_attack = false;
							}
							else{
								self->frame = 0;
								self->sprite = self->sprite_list.wallAttack;
								//run attack
								//slog("wallslide attack");
								self->in_attack = true;
								self->can_attack = false;
								self->is_wall_sliding = true;
							}
						}
						else if ((self->left_trigger&&self->l_wall_collision && ((self->in_air > 20) || self->falling == true)) || (self->right_trigger&&self->r_wall_collision && ((self->in_air > 20) || self->falling == true))){
							self->frame = 0;
							self->sprite = self->sprite_list.wallSlide;
							self->is_wall_sliding = true;
							//run attack
							//slog("wall slide");
						}
						else{
							//slog("jumping");
							self->frame = 0;
							self->sprite = self->sprite_list.jump;
						}
	}break;
	case(ES_Dash) : {
						if (self->in_attack == true){
							if (self->sprite != self->sprite_list.dashAttack){
								self->sprite = self->sprite_list.dashAttack;
							}
							//self->frame=frame;
						}
						else if (self->attack_trigger == true && self->can_attack == true && self->heldFrame < 9){
							self->frame = 0;
							self->sprite = self->sprite_list.dashAttack;
							self->attack(self);
							//run attack
							//slog("dash attack");
							self->in_attack = true;
							self->can_attack = false;
						}

						else{
							//slog("dashing");
							self->frame = 0;
							self->sprite = self->sprite_list.dash;
						}
	}break;
	case(ES_Dead) : {
						self->sprite = self->sprite_list.dying;
	}break;
	case(ES_Hit) : {
					   //slog("hit");
					   self->frame = 0;
					   self->invincibleFrame = 240;
					   self->sprite = self->sprite_list.hit;
	}break;
	case(ES_WarpIn) : {
						  //slog("hit");
						  //self->frame = 0;
						  self->sprite = self->sprite_list.warpIn;
	}break;
	case(ES_WarpOut) : {
						   //slog("hit");
						   //self->frame = 0;
						   self->sprite = self->sprite_list.warpOut;
						   play_soundeffect("../sounds/warp.wav", 0);
	}break;
	}


}
void gungirl_transition(Entity *self){
	if (self->transition){
		switch (self->transition_dir){
		case(Right) : {
						  self->position.x += .5;
		}break;
		case(Left) : {
						 self->position.x -= .5;
		}break;
		case(Up) : {
					   self->position.y -= .4;
		}break;
		case(Down) : {
						 self->position.y += .4;
		}break;
		}
	}
	else
		return;
}
void update_gungirl_ent(Entity *self){

	if (self->transition){
		gungirl_transition(self);
		return;
	}
	self->frame += 0.1;
	if (self->invincibleFrame > 0){
		self->invincibleFrame--;
		if (self->invincibleFrame % 3 == 1)
			self->color = vector4d(255, 255, 255, 0);
		else
			self->color = vector4d(255, 255, 255, 255);
	}
	if (self->is_held == true){
		self->heldFrame++;
	}
	if (self->upper_wall_collision){
		self->jumpFrame = 20;
		slog("upper wall collision");
	}
	if (self->actionFrame > 0)
		self->actionFrame--;
	if (self->health > 27)
		self->health = 27;
	if (self->health <= 0)
	{
		if (self->state != ES_Dead)
			self->onDeath(self);
	}
	//slog("position %f,%f", self->position.x, self->position.y);
	//slog("current weapon %i", self->weapons_list.currentWeapon);
	if (self->heldFrame > 30){
		if (self->heldFrame < 120){
			if (self->heldFrame % 9 == 1)
				self->color = vector4d(255, 255, 255, 0);
			else
				self->color = vector4d(255, 255, 255, 255);
		}
		else{
			if (self->heldFrame % 3 == 1)
				self->color = vector4d(255, 255, 255, 0);
			else
				self->color = vector4d(255, 255, 255, 255);
		}
	}
	update_hitbox_position(self);
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
	if (self->state == ES_Hit){
		//turn everthing off;

	}
	if (self->falling){
		self->maxjump = 0;
	}
	if (self->clip != 0){
		if (self->position.y < get_game_camera()->topLeftBounds.y)
			self->position.y = get_game_camera()->topLeftBounds.y;
		if (self->position.x<get_game_camera()->topLeftBounds.x)
			self->position.x = get_game_camera()->topLeftBounds.x;
	}
	if (self->state == ES_WarpOut){
		if (self->frame > 2){
			self->position.y -= 2;
			self->clip = 0;
			self->frame = 2;
		}
		//if outside bounds go to select screen/victory screen
		if (!in_camera_bounds(get_game_camera(), self)){
			clear_key();
			set_input_control(1);
			set_game_state(G_BossSelect, 0);
			return;
		}
	}
	if (!self->is_grounded){
		if (self->state == ES_WarpIn){
			if (self->sprite != self->sprite_list.warpIn){
				self->update_sprite(self);
			}
			if (self->frame > 0)
				self->frame = 0;
		}
		if (self->is_wall_sliding == false){
			if (self->state != ES_Dash){
				self->position.y += 1.5;  //might change to gravity
				self->velocity.y += 1.5;
			}
		}
		else{
			self->position.y += .5;  //might change to gravity
			self->velocity.y += .5;
			//slog("wall sliding");
		}
		if (self->state != ES_Hit && self->state != ES_WarpIn &&self->state != ES_Dash){
			if (self->sprite != self->sprite_list.warpOut || self->sprite != self->sprite_list.warpIn || self->sprite != self->sprite_list.jump || self->sprite != self->sprite_list.jumpAttack || self->sprite != self->sprite_list.wallSlide || self->sprite != self->sprite_list.wallAttack){
				self->state = ES_Jump;
				//slog("To jump");
				self->update_sprite(self);
			}
		}
	}
	if (self->state == ES_Hit || !self->left_trigger&&self->l_wall_collision || self->left_trigger&&!self->l_wall_collision || !self->right_trigger && self->r_wall_collision || self->right_trigger && !self->r_wall_collision || !self->r_wall_collision&&!self->l_wall_collision){
		self->is_wall_sliding = false;
	}
	if (self->is_grounded){
		self->jumped = false;
		self->is_wall_sliding = false;
		self->falling = false;
		self->in_air = 0;
		self->maxjump = 1;
		self->jumpFrame = 0;
		if (self->state == ES_Jump){
			self->state = ES_Idle;
			self->update_sprite(self);
		}
		if (self->state == ES_WarpIn){
			self->start_position = self->position;
			if (self->frame > 0.0&&self->frame<.2)
				play_soundeffect("../sounds/warp.wav", 0);
		}
	}

	if (self->jumped){
		self->in_air++;
	}

	//end frames
	if (self->frame > self->sprite->frames_per_line - 1){
		if (self->actionFrame <= 0){
			if (return_game_controller()->boss_state == false){
				self->state = ES_WarpOut;
				self->frame = 0;
				self->update_sprite(self);
			}
			if (self->state == ES_WarpOut){
				self->frame = self->sprite->frames_per_line - 1;
			}
			if (self->state == ES_WarpIn){
				self->state = ES_Idle;
				self->frame = 0;
				self->update_sprite(self);
			}
			if (self->in_attack){
				self->can_attack = true;
				self->in_attack = false;
				////slog("done");
				self->attacknum = 0;
				self->update_sprite(self);
			}
			if (self->sprite == self->sprite_list.hit){
				self->state = ES_Idle;
				self->frame = 0;
				self->update_sprite(self);
			}
			if (self->state == ES_Dead){
				self->frame = self->sprite->frames_per_line - 1;
				self->state = ES_Idle;
				self->update_sprite(self);
				respawn(self);
			}
			else{
				self->frame = 0;
				//slog("looping");
			}
		}
		else{
			if (self->state == ES_Dead){
				self->frame = self->sprite->frames_per_line;

			}
			else{
				self->frame = 0;
			}
		}
	}

}
void init_gungirl_ent(Entity *self, int ctr){
	self->state = ES_WarpIn;
	//self->update_sprite = update_gungirl_sprite;
	self->controlling = ctr;
	self->frame = 0;
	self->position = vector2d(100, 500);
	self->color = vector4d(255, 255, 255, 255);
	self->sprite_list.idle = gf2d_sprite_load_all("../images/test/idle.png", 32, 32, 1);
	self->sprite_list.run = gf2d_sprite_load_all("../images/test/run.png", 32, 32, 4);
	self->sprite_list.jump = gf2d_sprite_load_all("../images/test/jump.png", 32, 32, 2);
	self->sprite_list.wallSlide = gf2d_sprite_load_all("../images/test/wallSlide.png", 32, 32, 1);
	self->sprite_list.wallAttack = gf2d_sprite_load_all("../images/test/wallAttack.png", 32, 32, 3);
	self->sprite_list.dash = gf2d_sprite_load_all("../images/test/dash.png", 32, 32, 1);
	//self->sprite_list->dying;
	self->sprite_list.idleAttack = gf2d_sprite_load_all("../images/test/idleAttack.png", 32, 32, 3);
	self->sprite_list.runAttack = gf2d_sprite_load_all("../images/test/runAttack.png", 32, 32, 4);
	self->sprite_list.jumpAttack = gf2d_sprite_load_all("../images/test/jumpAttack.png", 32, 32, 3);
	self->sprite_list.dashAttack = gf2d_sprite_load_all("../images/test/dashAttack.png", 32, 32, 3);
	self->sprite_list.hit = gf2d_sprite_load_all("../images/test/hit.png", 32, 32, 8);
	self->sprite_list.warpIn = gf2d_sprite_load_all("../images/test/warpIn.png", 32, 32, 3);
	self->sprite_list.warpOut = gf2d_sprite_load_all("../images/test/warpOut.png", 32, 32, 3);
	self->sprite_list.dying = gf2d_sprite_load_all("../images/test/effect/boom.png", 56, 56, 15);
	self->sprite_list.dying->sprite_offsety = 18;
	//self->sprite_list->special1;
	self->think = gungirl_think;
	self->movementspeed = 1;
	self->dashspeed = 1.5;
	self->maxjump = 1;
	self->health = 27;
	self->healthmax = 27;
	self->update_sprite = update_gungirl_sprite;
	self->type = ES_Player;
	self->is_grounded = false;
	self->can_attack = true;
	self->attack_trigger = false;
	self->is_held = false;
	self->l_wall_collision = false;
	self->r_wall_collision = false;
	self->right_trigger = false;
	self->left_trigger = false;
	self->jumped = false;
	self->wall_jumped = false;
	self->falling = false;
	self->transition = false;
	self->upper_wall_collision = false;
	self->in_air = 0;
	self->invincibleFrame = 0;
	self->action = none;
	self->attacknum = 0;
	self->attack = gungirl_attack;
	self->transition_function = gungirl_transition;
	set_hitbox(self, self->position.x, self->position.y, 24, 24, 4, 3);
	self->update_ent = update_gungirl_ent;
	self->sprite = self->sprite_list.idle;
	self->damage = gungirl_damage;
	self->weapons_list.weaponsList = (int*)gfc_allocate_array(sizeof(int), 4);
	memset(self->weapons_list.weaponsList, -1, sizeof(int)* 4);
	self->weapons_list.weaponsList[0] = 1;
	self->weapons_list.currentWeapon = 0;
	self->getPowerUp = gungirl_get_ability;
	self->attackdmg = 3;
	self->clip = 1;
	self->actionFrame = 0;
	self->victory = gungirl_victory;
	self->onDeath = gungirl_death;
	self->reset = gungirl_respawn;
}
void gungirl_set_position(Entity *self, Vector2D position){
	self->position = position;
}
void gungirl_displacement(Entity *self, Vector2D position){
	self->velocity = position;
	self->position.x += self->velocity.x;
	self->position.y += self->velocity.y;
}
void gungirl_get_inputs(Entity *self, const Uint8 * keys){
	if (self->controlling == 1 && self->transition == false && self->state != ES_Dead){
		//slog("inputs");
		
		Vector2D velocity;
		if (self->state != ES_Dash&&self->state != ES_Hit&&self->state != ES_Special){
			if (keys[SDL_SCANCODE_RIGHT]){
				self->right_trigger = true;
				if (self->is_grounded){
					self->state = ES_Running;
				}
				if (!self->r_wall_collision)
					dx_a = 1;
				else
					dx_a = 0;
				self->dir = Right;
				self->action = movement;
			}
			if (keys[SDL_SCANCODE_LEFT]){
				self->left_trigger = true;
				if (self->is_grounded){
					self->state = ES_Running;
				}
				if (!self->l_wall_collision)
					dx_a = -1;
				else
					dx_a = 0;
				self->dir = Left;
				self->action = movement;

			}
			if (self->is_dashing){
				gungirl_displacement(self, vector2d((dx_a*(self->movementspeed)), 0));
			}
			else{
				gungirl_displacement(self, vector2d((dx_a*(self->movementspeed)), 0));
			}
		}

		if (keys[SDL_SCANCODE_S]){
			self->action = jump;
			self->jumpFrame++;
		}
		if (keys[SDL_SCANCODE_D]){
			if (!self->l_wall_collision&&!self->r_wall_collision){
				if (self->dashFrame > 50){
					self->is_dashing = false;
					if (self->is_grounded == true){
						//self->action = none;
					}
					else{
						self->action = jump;
					}
				}
				else{
					self->action = dash;
					self->dashFrame++;
				}
			}
		}

		//single button events
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_MOUSEMOTION:
				break;
			case SDL_QUIT:
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym){
				case SDLK_a: {
								 if (self->is_held == false){
									 self->attack_trigger = true;
									 //slog("attack trigger pressed");
								 }
								 else{
									 self->attack_trigger = false;
								 }
				}
					break;
				case SDLK_w:{
								if (self->weapons_list.currentWeapon == 3){
									self->weapons_list.currentWeapon = 0;
								}
								else{
									for (int i = self->weapons_list.currentWeapon; i <= 3; i++){
										if (self->weapons_list.weaponsList[i + 1] > -1){
											self->weapons_list.currentWeapon = i + 1;
											self->energy = self->weapons_list.weaponsList[i + 1];
											slog("currentWeapon %i", i + 1);
											return;
										}
										if (i == 3){
											self->weapons_list.currentWeapon = 0;
											self->energy = 0;
											return;
										}
									}
								}
				}break;
				case SDLK_p:{
								return_game_controller()->pause = true;
				}break;
				}break;
			case SDL_KEYUP:

				switch (event.key.keysym.sym)
				{
				case SDLK_a:{
								self->action = none;
								self->is_held = false;
								if (self->heldFrame > 3){
									self->color = vector4d(255, 255, 255, 255);
								}
								if (self->heldFrame > 60){
									//fire different type of attack
									//slog("big pew");
									if (self->can_attack){
										self->attack_trigger = true;
										self->attacknum = 1;
										self->heldFrame = 0;
										self->color = vector4d(255, 255, 255, 255);
										self->update_sprite(self);
									}
								}
								self->attack_trigger = false;
								//update attack frames
								self->heldFrame = 0;
				}break;
				case SDLK_d:{
								self->dashFrame = 0;
								self->is_dashing = false;
								self->right_trigger = false;
								self->action = none;
								if (self->is_grounded == true){
									self->state = ES_Idle;
								}
								else{
									self->state = ES_Jump;
								}
								//self->state = ES_Idle;
				}break;
				case SDLK_s:{
								self->jumpFrame = 0;
								self->wall_jumped = false;
								self->action = none;
								if (self->maxjump > 0 && !self->is_grounded){
									self->maxjump--;
								}
								//self->state = ES_Idle;
				}break;

				case SDLK_LEFT:{
								   dx_a = 0;
								   dy_a = 0;
								   self->left_trigger = false;
								   if (self->state == ES_Running){
									   self->state = ES_Idle;
									   self->action = none;
									   prev_action_a = none;
									   self->update_sprite(self);
								   }
				} break;
				case SDLK_RIGHT: {
									 dx_a = 0;
									 dy_a = 0;
									 self->right_trigger = false;
									 if (self->state == ES_Running){
										 self->state = ES_Idle;
										 self->action = none;
										 prev_action_a = none;
										 self->update_sprite(self);
									 }
				} break;
				default:
					break;
				}
				break;
			}
		}
		if (keys[SDL_SCANCODE_A]){
			self->is_held = true;
		}
		if (self->state != ES_Hit)
			self->think(self);
	}
}

void create_gungirl_projectile(Entity *self, float speed, float dmg, int type){
	if (type > 1){
		if (self->energy <= 1)
			return;
	}
	Entity *projectile = gf2d_entity_new();
	Projectiles proj;
	projectile->attackdmg = dmg;
	proj.parentType = ES_Player;
	init_projectile_ent(projectile, -1);

	int dir = (self->dir == Right) ? 1 : -1;
	if (self->is_wall_sliding)
		dir *= -1;
	switch (type){
	case 0:{
			   proj.direction = vector2d(dir*speed * 1, 0);
			   proj.destroyOnCollision = true;
			   proj.destroyeffect = -1;
			   proj.aliveFrame = -1;
			   proj.gravity = false;
			   proj.destroyOnSurface = false;
			   proj.stick = 0;
			   projectile->sprite = gf2d_sprite_load_all("../images/test/projectile/lemon.png", 8, 5, 1);
			   slog("lemons");
			   projectile->position.x = (dir == 1) ? self->position.x + self->hitbox.offsetx + self->hitbox.w : self->position.x + self->hitbox.offsetx - 3;
			   projectile->position.y = self->position.y + self->hitbox.offsety + 7;
			   set_hitbox(projectile, projectile->position.x, projectile->position.y, 8, 5, 0, 0);
			   projectile->proj_data = proj;
			   play_soundeffect("../sounds/lemon.wav", 0);
	}break;
	case 1:{ slog("bigger lemon");
		proj.direction = vector2d(dir*speed * 1, 0);
		proj.destroyOnCollision = true;
		proj.aliveFrame = -1;
		proj.destroyeffect = -1;
		proj.gravity = false;
		proj.destroyOnSurface = false;
		proj.stick = 0;
		projectile->sprite = gf2d_sprite_load_all("../images/test/projectile/big_blast.png", 31, 30, 5);
		if (!(self->is_wall_sliding))
			projectile->flip = self->flip;
		else{
			projectile->flip.y = 0;
			if (dir == 1)
				projectile->flip.x = 0;
			else
				projectile->flip.x = 1;
		}
		projectile->position.x = (dir == 1) ? self->position.x + self->hitbox.offsetx + self->hitbox.w - 7 : self->position.x + self->hitbox.offsetx - 3;
		projectile->position.y = self->position.y + self->hitbox.offsety - 3;
		set_hitbox(projectile, projectile->position.x, projectile->position.y, 31, 30, 0, 0);
		projectile->proj_data = proj;
	}break;
	case 2:{
			   slog("fire pew");
			   proj.direction = vector2d(dir*speed * 1, 0);
			   proj.destroyOnCollision = true;
			   proj.aliveFrame = -1;
			   proj.gravity = false;
			   proj.destroyOnSurface = false;
			   proj.stick = 0;
			   proj.destroyeffect = -1;
			   gungirl_update_list(self, -5);
			   projectile->sprite = gf2d_sprite_load_all("../images/test/projectile/fire_weapon.png", 24, 24, 2);
			   if (!(self->is_wall_sliding))
				   projectile->flip = self->flip;
			   else{
				   projectile->flip.y = 0;
				   if (dir == 1)
					   projectile->flip.x = 0;
				   else
					   projectile->flip.x = 1;
			   }
			   projectile->position.x = (dir == 1) ? self->position.x + self->hitbox.offsetx + self->hitbox.w - 7 : self->position.x + self->hitbox.offsetx - 3;
			   projectile->position.y = self->position.y + self->hitbox.offsety - 3;
			   set_hitbox(projectile, projectile->position.x, projectile->position.y, 24, 24, 0, 0);
			   projectile->proj_data = proj;
	}break;
	case 3:{
			   slog("metal pew");
			   play_soundeffect("../sounds/metalblade.wav", 0);
			   proj.direction = vector2d(dir*speed * 1, 0);
			   proj.destroyOnCollision = true;
			   proj.aliveFrame = -1;
			   proj.gravity = false;
			   proj.destroyOnSurface = false;
			   proj.stick = 0;
			   proj.destroyeffect = -1;
			   gungirl_update_list(self, -1);
			   projectile->sprite = gf2d_sprite_load_all("../images/test/projectile/metalblade_weapon.png", 16, 16, 2);
			   if (!(self->is_wall_sliding))
				   projectile->flip = self->flip;
			   else{
				   projectile->flip.y = 0;
				   if (dir == 1)
					   projectile->flip.x = 0;
				   else
					   projectile->flip.x = 1;
			   }
			   projectile->position.x = (dir == 1) ? self->position.x + self->hitbox.offsetx + self->hitbox.w - 7 : self->position.x + self->hitbox.offsetx - 3;
			   projectile->position.y = self->position.y + self->hitbox.offsety + 3;
			   set_hitbox(projectile, projectile->position.x, projectile->position.y, 24, 24, 0, 0);
			   projectile->proj_data = proj;
	}break;
	case 4:{
			   slog("crash pew");
			   proj.direction = vector2d(dir*1, 0);
			   proj.destroyOnCollision = true;
			   proj.destroyOnSurface = false;
			   projectile->actionFrame = 300;
			   proj.gravity = 0;
			   proj.destroyeffect = 1;
			   proj.stick = true;
			   gungirl_update_list(self, -4);
			   projectile->sprite = gf2d_sprite_load_all("../images/test/projectile/crashbomb_weapon.png", 13, 15, 3);
			   if (!(self->is_wall_sliding))
				   projectile->flip = self->flip;
			   else{
				   projectile->flip.y = 0;
				   if (dir == 1)
					   projectile->flip.x = 0;
				   else
					   projectile->flip.x = 1;
			   }
			   projectile->position.x = (dir == 1) ? self->position.x + self->hitbox.offsetx + self->hitbox.w - 7 : self->position.x + self->hitbox.offsetx - 3;
			   projectile->position.y = self->position.y + self->hitbox.offsety + 7;
			   set_hitbox(projectile, projectile->position.x, projectile->position.y, 13, 15, 0, 0);
			   projectile->proj_data = proj;
	}break;
	}
}

void gungirl_get_ability(Entity* self, int index){
	self->weapons_list.weaponsList[index] = 28;
	slog("got %i", index);
}
void gungirl_update_list(Entity* self, int amount){
	self->energy += amount;
	if (self->energy < 0)
		self->energy = 0;
	self->weapons_list.weaponsList[self->weapons_list.currentWeapon] += amount;
	if (self->weapons_list.weaponsList[self->weapons_list.currentWeapon] <= 0)
		self->weapons_list.weaponsList[self->weapons_list.currentWeapon] = 1;
	if (self->weapons_list.weaponsList[self->weapons_list.currentWeapon] >= 28)
		self->weapons_list.weaponsList[self->weapons_list.currentWeapon] = 28;
}

void gungirl_victory(Entity *self){
	self->left_trigger = false;
	self->right_trigger = false;
	self->is_dashing = false;
	self->state = ES_Idle;
	self->actionFrame = 600;
}

void gungirl_death(Entity* self){
	if (self->state != ES_Dead){
		self->state = ES_Dead;
		self->update_sprite(self);
		stop_bgm();
		self->actionFrame = 360;
		play_soundeffect("../sounds/dead.wav", 0);
	}
}

void gungirl_reset(Entity* self){
	clear_key();
	self->state = ES_Idle;
	self->update_sprite(self);
	dx_a = 0;
	dy_a = 0;
	self->frame = 0;
	self->heldFrame = 0;
	self->color = vector4d(255, 255, 255, 255);
	self->movementspeed = 1;
	self->dashspeed = 1.5;
	self->maxjump = 1;	
	self->is_grounded = false;
	self->can_attack = true;
	self->attack_trigger = false;
	self->is_held = false;
	self->l_wall_collision = false;
	self->r_wall_collision = false;
	self->right_trigger = false;
	self->left_trigger = false;
	self->jumped = false;
	self->wall_jumped = false;
	self->falling = false;
	self->transition = false;
	self->upper_wall_collision = false;
	self->is_dashing = false;
	self->dashFrame = 0;
	self->in_air = 0;
	self->invincibleFrame = 0;
	self->action = none;
	self->attacknum = 0;
	self->clip = 1;
	self->actionFrame = 0;
	self->action = none;
}


void gungirl_respawn(Entity* self){
	clear_key();
	self->health = 27;
	self->healthmax = 27;
	self->state = ES_Idle;
	self->update_sprite(self);
	dx_a = 0;
	dy_a = 0;
	self->frame = 0;
	self->heldFrame = 0;
	self->color = vector4d(255, 255, 255, 255);
	self->movementspeed = 1;
	self->dashspeed = 1.5;
	self->maxjump = 1;
	self->is_grounded = false;
	self->can_attack = true;
	self->attack_trigger = false;
	self->is_held = false;
	self->l_wall_collision = false;
	self->r_wall_collision = false;
	self->right_trigger = false;
	self->left_trigger = false;
	self->jumped = false;
	self->wall_jumped = false;
	self->falling = false;
	self->transition = false;
	self->upper_wall_collision = false;
	self->is_dashing = false;
	self->dashFrame = 0;
	self->in_air = 0;
	self->invincibleFrame = 0;
	self->action = none;
	self->attacknum = 0;
	self->attackdmg = 3;
	self->clip = 1;
	self->actionFrame = 0;
	self->action = none;
}