#include "g_gungirl.h";

SDL_Event event;
float dx_a, dy_a = 0;

void gungirl_think(Entity *self);
void gungirl_attack(Entity *self){
	if (self->can_attack){
		switch (self->attacknum){
		case(0):
			//fire gun
		case(1):
			   //slide?
		default:
			//nothing?
		}
	}
}
void gungirl_special(Entity *self){
	if (self->can_special){
		switch (self->specialnum){
		case(0) :
			//fire gun
		case(1) :
				//slide?
		default:
			//nothing?
		}
	}

}
void gungirl_dash(Entity *self){

}
void update_gungirl_sprite(Entity *self){
	switch (self->state){
	case(ES_Idle) : {
						self->sprite = self->sprite_list->idle;
	}
	case(ES_Running) : {
						self->sprite = self->sprite_list->run;
	}
	case(ES_Attacking) : {
							 if (self->attacknum == 0){
								 self->sprite = self->sprite_list->attack1;
							 }
	}
	case(ES_Jump) : {
						self->sprite = self->sprite_list->jump;
	}
	case(ES_Dash) : {
						 self->sprite = self->sprite_list->dash;
	}
	case(ES_Dying) : {
						self->sprite = self->sprite_list->dying;
	}
	}


}
void update_gungirl_ent(Entity *self){
	self->frame++;
	switch (self->dir){
	case(Right) :
	{
					self->flip = vector2d(0, 0);
	}
	case(Left) :
	{
				   self->flip = vector2d(1, 0);
	}
	}
	if (!self->is_grounded){
		self->position.y -= .2;  //might change to gravity
	}
	if (self->is_held == true){
		self->heldFrame++;
	}
	if (self->frame > self->sprite->frames_per_line){
		self->frame = 0;
	}
}
void init_gungirl_ent(Entity *self, int ctr){
	self->state = ES_Idle;
	self->sprite_list->idle;
	self->sprite_list->run;
	self->sprite_list->jump;
	self->sprite_list->dash;
	self->sprite_list->dying;
	self->sprite_list->attack1;
	self->sprite_list->special1;
}
void gungirl_set_position(Entity *self, Vector2D position){
	self->position = position;
}
void gungirl_displacement(Entity *self, Vector2D position){
	self->position.x += position.x;
	self->position.y += position.y;
}
void gungirl_get_inputs(Entity *self, const Uint8 * keys){
	if (self->controling == 1){
		if (self->state != ES_Attacking&&self->state != ES_Dash&&self->state != ES_Hit&&self->state != ES_Special){
			if (keys[SDL_SCANCODE_RIGHT]){
				self->state = ES_Running;
				dx_a = -1;
				self->dir = Right;
				self->action = movement;
			}
			if (keys[SDL_SCANCODE_LEFT]){
				self->state = ES_Running;
				dx_a = 1;
				self->dir = Left;
				self->action = movement;
			}
			gungirl_displacement(self, vector2d((dx_a*(self->movementspeed)), (dy_a*(self->movementspeed))));
		}
		if (keys[SDL_SCANCODE_A]){
			self->is_held = true;
			//self->action = attack;
		}
		if (keys[SDL_SCANCODE_S]){
			self->action = jump;
		}
		if (keys[SDL_SCANCODE_D]){
			self->action = dash;
		}
		if (keys[SDL_SCANCODE_W]){
			self->action = special;
		}
		self->think(self);
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
				case(SDLK_a):
					self->action = attack;
				}
				break;
			case SDL_KEYUP:				
				
				switch (event.key.keysym.sym)
				{
				case SDLK_a:{ 
								self->action = none; 
								self->is_held = false; 
								//update attack frames
								self->heldFrame = 0;
				}break;
				case SDLK_d: self->action = none; break;
				case SDLK_f: self->action = none; self->state = ES_Idle; self->update_sprite(self); break;
				//case SDLK_UP: up_trigger_arrow = false; break;
				//case SDLK_DOWN: down_trigger_arrow = false; break;
				case SDLK_LEFT:{ 
								   dx_a, dy_a = 0;
								   if (self->state == ES_Running){
									   self->state = ES_Idle;
									   self->action = none;
									   self->update_sprite(self);
								   }
				} break;
				case SDLK_RIGHT: {
									 dx_a, dy_a = 0;
									 if (self->state == ES_Running){
										 self->state = ES_Idle;
										 self->action = none;
										 self->update_sprite(self);
									 }
				} break;
				default:
					break;
				}
				break;
			}
		}
	}
void create_gungirl_projectile(Entity *self, float speed, float dmg, int type);