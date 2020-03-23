#include "g_projectile.h"
#include "simple_logger.h"


void update_projectile_ent(Entity *self){
	self->frame += .1;
	if (self->frame > self->sprite->frames_per_line - 1){
		self->frame = 0;
	}
	if (self->actionFrame > 0){
		self->actionFrame--;
	}
	if (self->actionFrame == 0){
		//destroy self
		set_to_zero_ent(self->Ent_ID);
	}
	if (self->proj_data.heightTime > 0)
		self->proj_data.heightTime--;

	else if (self->proj_data.heightTime == 0){
		self->proj_data.direction.y = 0;
	}

	if (self->clip == 1){
		if (!self->is_grounded){
			self->position.y += 1;
		}
		else{
			self->proj_data.direction.x = 0;
		}
	}
	update_hitbox_position(self);
	projectile_displacement(self,self->proj_data.direction);
	//check if inside camera bounds
}
void init_projectile_ent(Entity *self,int actionFrame){
	self->frame = 0;
	self->clip = 0;
	self->type = ES_Projectile;
	self->color = vector4d(255, 255, 255, 255);
	self->update_ent = update_projectile_ent;
	self->move = projectile_displacement;
	self->actionFrame = actionFrame;
}
void set_projectile_position(Entity *self, Vector2D position){
	self->position = position;
}
void projectile_displacement(Entity *self, Vector2D direction){
	//slog("moving");
	self->position.x += direction.x;
	self->position.y += direction.y;
}