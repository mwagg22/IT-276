#include "g_hazards.h"



void update_hazard_ent(Entity *self);
void init_hazard_ent(Entity *self, int type){
	self->type = ES_Hazard;
	self->frame = 0;
	self->l_wall_collision = false;
	self->r_wall_collision = false;
	self->color = vector4d(255, 255, 255, 255);
	switch (type){
	case(0) : {
				  self->update_ent = update_slider_hazard;
				  set_hitbox(self, self->position.x, self->position.y, 16, 16, 0, 0);
				  self->sprite = gf2d_sprite_load_all("../images/test/hazards/slider.png", 16, 16, 2);
				  self->dir = 1;
				  self->can_attack = true;
	}break;
	case(1) : {
				  self->update_ent = update_flamethrower_hazard;
				  self->sprite = gf2d_sprite_load_all("../images/test/hazards/flamethrower.png", 16, 81, 8);
				  set_hitbox(self, self->position.x, self->position.y, 16, 81, 0, 0);
				  self->position = vector2d(100, 720);
				  self->start_position = self->position;
				  self->can_attack = false;
	}break;
	case(2) : {
				  self->update_ent = update_fallingrocks_hazard; 
				  set_hitbox(self, self->position.x, self->position.y, 32, 32, 0, 0);
				  self->position = vector2d(823, 500);
				  self->start_position = self->position;
				  self->can_attack = true;
				  self->sprite = gf2d_sprite_load_all("../images/test/hazards/rock.png", 32, 32, 1);
	}break;
	}
	
}
void set_hazard_position(Entity *self, Vector2D position){
	self->position = position;
}
void hazard_displacement(Entity *self, Vector2D direction){
	self->velocity = direction;
	self->position.x += direction.x;
	self->position.y += direction.y;
}

//hazard actions
void update_flamethrower_hazard(Entity* self){
	if (self->actionFrame > 0)
		self->actionFrame--;
	if (self->actionFrame <= 0){
		self->frame += .1;
		if (self->frame > 3){
			self->can_attack = true;
		}
		if (self->frame > self->sprite->frames_per_line - 1){
			self->frame = 0;
			self->can_attack = false;
			self->actionFrame = 60;//cool down
		}
	}
}
void update_fallingrocks_hazard(Entity* self){
	self->frame += .1;
	hazard_displacement(self, vector2d(0, 3));
	if (self->frame > self->sprite->frames_per_line - 1){
		self->frame = 0;
	}

}
void update_slider_hazard(Entity* self){
	if (!self->is_grounded){
		self->position.y += 1.5;  //might change to gravity
	}
	if (abs(get_player_entity()->position.y - self->position.y)<40)
		self->movementspeed = 1.5;
	else
		self->movementspeed = 1;
	if (self->l_wall_collision){
		self->r_wall_collision = false;
		self->dir = 1;
	}
	if (self->r_wall_collision){
		self->l_wall_collision = false;
		self->dir = -1;
	}
	hazard_displacement(self, vector2d(self->dir*self->movementspeed, 0));
	if (self->frame > self->sprite->frames_per_line - 1){
		self->frame = 0;
	}
}