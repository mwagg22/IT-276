#include "g_gungirl.h"
#include "simple_logger.h"

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
		if (self->action != prev_action_a || self->attack_trigger == true && self->can_attack == true){
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
		if (self->action != prev_action_a || self->jumpFrame>0){
			gungirl_jump(self);
			self->update_sprite(self);
			prev_action_a = self->action;
		}
		break;
	default:
		if (self->action != prev_action_a || self->attack_trigger == true && self->can_attack==true){
			self->update_sprite(self);
			prev_action_a = self->action;
		}
		break;
	}

}
void gungirl_attack(Entity *self){
	switch (self->attacknum){
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
		//fire stronger gun?
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
	case(2) :
		//fire stronger gun?
		break;
	default:
		//nothing?
		break;
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
	if (self->dashFrame < 60){
		if (self->dir==Right)
			gungirl_displacement(self, vector2d(self->dashspeed, 0));
		else
			gungirl_displacement(self, vector2d(-self->dashspeed, 0));
	}
	else{
		self->is_dashing = false;
		self->state = ES_Idle;
	}
}
void gungirl_jump(Entity *self){
	self->is_grounded = false;
	if (self->jumpFrame < 120){
		self->state = ES_Jump;
		gungirl_displacement(self, vector2d(0, -3));
	}
}
void update_gungirl_sprite(Entity *self){
	switch (self->state){
		
	case(ES_Idle) : {slog("idle time");
						if (self->in_attack == true){
							if (self->sprite != self->sprite_list.attack1){
								self->frame = 1;
								slog("idle attack");
								self->sprite = self->sprite_list.attack1;
							}
							//self->frame=frame;
						}
						else if (self->attack_trigger == true && self->can_attack == true && self->heldFrame < 9 ){
							self->sprite = self->sprite_list.attack1;
							//run attack
							slog("attacking");
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
							   if (self->sprite != self->sprite_list.attack2){
								   self->sprite = self->sprite_list.attack2;
							   }
							   //self->frame=frame;
						   }
						   else if (self->attack_trigger == true && self->can_attack == true && self->heldFrame < 9){
							   self->frame = 0;
							   self->sprite = self->sprite_list.attack2;
							   //run attack
							   slog("attacking dos");
							   self->in_attack = true;
							   self->can_attack = false;
						   }
						   
						   else{
							   slog("running");
							   self->frame = 0;
							   self->sprite = self->sprite_list.run;
						   }
	}break;
	case(ES_Attacking) : {
							 
	}break;
	case(ES_Jump) : {
						if (self->in_attack == true){
							if (self->sprite != self->sprite_list.attack3){
								self->sprite = self->sprite_list.attack3;
							}
							//self->frame=frame;
						}
						else if (self->attack_trigger == true && self->can_attack == true){
							self->frame = 0;
							self->sprite = self->sprite_list.attack3;
							//run attack
							slog("jump attack");
							self->in_attack = true;
							self->can_attack = false;
						}
						 
						else{
							slog("jumping");
							self->frame = 0;
							self->sprite = self->sprite_list.jump;
						}
	}break;
	case(ES_Dash) : {
						if (self->in_attack == true){
							if (self->sprite != self->sprite_list.attack3){
								self->sprite = self->sprite_list.attack3;
							}
							//self->frame=frame;
						}
						else if (self->attack_trigger == true && self->can_attack == true){
							self->frame = 0;
							self->sprite = self->sprite_list.attack3;
							//run attack
							slog("dash attack");
							self->in_attack = true;
							self->can_attack = false;
						}

						else{
							slog("dashing");
							self->frame = 0;
							self->sprite = self->sprite_list.dash;
						}
	}break;
	case(ES_Dying) : {
						 self->sprite = self->sprite_list.dying;
	}break;
	}


}
void update_gungirl_ent(Entity *self){
	self->frame+=0.1;
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
		self->state = ES_Jump;
	}
	if (self->position.y >= 500){
		self->is_grounded = true;
		if (self->state == ES_Jump){
			self->state = ES_Idle;
			self->update_sprite(self);
		}
	}
	if (self->is_held == true){
		self->heldFrame++;
	}
	//end frames
	if (self->frame > self->sprite->frames_per_line-1){
		if (self->in_attack){
				self->can_attack = true;
				self->in_attack = false;
				//slog("done");
				slog("held");
				self->update_sprite(self);
			
		}

	else
		self->frame = 0;
	}
	
}
void init_gungirl_ent(Entity *self, int ctr){
	self->state = ES_Idle;
	//self->update_sprite = update_gungirl_sprite;
	self->controlling = ctr;
	self->frame = 0;
	self->position = vector2d(100, 500);
	self->color = vector4d( 255, 255, 255, 200 );
	self->sprite_list.idle = gf2d_sprite_load_all("../images/test/idle.png", 100, 100, 1);
	self->sprite_list.run = gf2d_sprite_load_all("../images/test/run.png", 100, 100, 4);
	self->sprite_list.jump = gf2d_sprite_load_all("../images/test/jump.png", 100, 100, 2);
	self->sprite_list.dash = gf2d_sprite_load_all("../images/test/dash.png", 100, 100, 1);
	//self->sprite_list->dying;
	self->sprite_list.attack1 = gf2d_sprite_load_all("../images/test/attack1.png", 100, 100, 3);
	self->sprite_list.attack2 = gf2d_sprite_load_all("../images/test/attack2.png", 100, 100, 8);
	self->sprite_list.attack3 = gf2d_sprite_load_all("../images/test/attack3.png", 100, 100, 3);
	//self->sprite_list->special1;
	self->think = gungirl_think;
	self->movementspeed = 1;
	self->dashspeed = 3;
	self->update_sprite = update_gungirl_sprite;
	self->is_grounded = true;
	self->can_attack = true;
	self->attack_trigger = false;
	self->is_held = false;
	self->action = none;
	self->update_ent = update_gungirl_ent;
	self->sprite = self->sprite_list.idle;
}
void gungirl_set_position(Entity *self, Vector2D position){
	self->position = position;
}
void gungirl_displacement(Entity *self, Vector2D position){
	self->position.x += position.x;
	self->position.y += position.y;
}
void gungirl_get_inputs(Entity *self, const Uint8 * keys){
	if (self->controlling == 1){
		//slog("inputs");
		if (self->state != ES_Dash&&self->state != ES_Hit&&self->state != ES_Special){
			if (keys[SDL_SCANCODE_RIGHT]){
				if (self->is_grounded){
					self->state = ES_Running;
				}
				dx_a = 1;
				self->dir = Right;
				self->action = movement;
			}
			if (keys[SDL_SCANCODE_LEFT]){
				if (self->is_grounded){
					self->state = ES_Running;
				}
				dx_a = -1;
				self->dir = Left;
				self->action = movement;
			}
			if (self->is_dashing){
				gungirl_displacement(self, vector2d((dx_a*(self->dashspeed)), 0));
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
			self->action = dash;
			self->dashFrame++;
		}
		if (keys[SDL_SCANCODE_W]){
			self->action = special;
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
									 slog("attack trigger pressed");
								 }
								 else{
									 self->attack_trigger = false;
								 }
				}
					break;
				}
				break;
			case SDL_KEYUP:

				switch (event.key.keysym.sym)
				{
				case SDLK_a:{
								self->action = none;
								self->is_held = false;
								if (self->heldFrame > 60){
									//fire different type of attack
									slog("big pew");
								}
								self->attack_trigger = false;
								//update attack frames
								self->heldFrame = 0;
				}break;
				case SDLK_d:{
								self->dashFrame = 0;
								self->is_dashing = false;
								self->action = none;
								self->state = ES_Idle;
				}break;
				case SDLK_s:{
								self->jumpFrame = 0;
								self->action = none;
								//self->state = ES_Idle;
				}break;
				//case SDLK_f: self->action = none; self->state = ES_Idle; self->update_sprite(self); break;
					//case SDLK_UP: up_trigger_arrow = false; break;
					//case SDLK_DOWN: down_trigger_arrow = false; break;
				case SDLK_LEFT:{
								   dx_a = 0;
								   dy_a = 0;
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
		self->think(self);
	}
}
//void create_gungirl_projectile(Entity *self, float speed, float dmg, int type);