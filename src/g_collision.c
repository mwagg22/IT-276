#include "g_collision.h"
#include "simple_logger.h"
#include <math.h>
void collision_check(Entity *ents, Uint32 entity_max){
	for (int i = 0; i < entity_max-1;i++){
		for (int j = 1; j < entity_max; j++){
			if (check_collision(&ents[i], &ents[j])){
				handle_collision(&ents[i], &ents[j]);
				handle_collision(&ents[j], &ents[i]);
			}
		}
	}
}
bool check_collision(Entity *self, Entity *other){
	return(
		self->position.x + self->hitbox.x >= other->position.x + other->hitbox.x &&
		self->position.x + self->hitbox.x  < other->position.x + other->hitbox.x &&
		self->position.y + self->hitbox.y >= other->position.y + other->hitbox.y &&
		self->position.y + self->hitbox.y  < other->position.y + other->hitbox.y );
}

void handle_collision(Entity *self, Entity *other){
	//do stuff
	switch (self->type){
	case ES_Player:{
	   int dir = (other->position.x > self->position.x) ? -1 : 1;
	    Vector2D kick = vector2d(5 * dir, 0);
		if (self->invincibleFrame == 0){
			if (other->type == ES_Projectile){
				if (other->parent->type != ES_Player)
					handle_projectile_collision(self, other, kick);
			}
			else if (other->type == ES_Hazard){
				handle_hazard_collision(self, other, kick);
			}
			else if (other->type == ES_Enemy){
				self->damage(self, 3, kick);
			}
		}
		if (other->type == ES_Item){
			handle_item_collision(self, other);
		}
	}break;
	case ES_Enemy:{
		int dir = (other->position.x > self->position.x) ? -1 : 1;
		Vector2D kick = vector2d(5 * dir, 0);
			if (other->type == ES_Projectile){
				//damage
					//handle_projectile_collision(self, other, kick);
					}
			else if (other->type == ES_Hazard){
							 //die instant
					}
	}break;
	case ES_Projectile:{
						   if (other->type == ES_Player){
							   //destroy self
						   }
						   if (other->type == ES_Enemy){
							   //destroy self
						   }
	}
	}
}

bool check_hit_collision(Entity *hitlist, Entity *other){

}
void handle_hit_collision(Entity *self, Entity *other, Vector2D kick){
	//self->damage(self, other->attackdmg,);
}

void handle_projectile_collision(Entity *self, Entity *other,Vector2D kick){
	self->damage(self, other->attackdmg,kick);
	if (other->proj_data.destroyOnCollision){
		set_to_zero_ent(other->Ent_ID);
	}
}
void handle_hazard_collision(Entity *self, Entity *other, Vector2D kick){
	
}

void check_tile_collision(Entity *self, int** tiles){
	int left_bound = ((self->position.x + self->hitbox.offsetx) / 16.0);//x
	int right_bound = ((self->position.x + self->hitbox.w + self->hitbox.offsetx) / 16.0);//x
	int top_bound = ((self->position.y + self->hitbox.offsety) / 16);//y
	int bottom_bound = ((self->position.y + self->hitbox.h + self->hitbox.offsety) / 16.0);//y
	int collision_ctr = 0;
	if (left_bound < 0)left_bound = 0;
	if (right_bound < 0)right_bound = 0;
	if (top_bound < 0)top_bound = 0;
	if (bottom_bound < 0)bottom_bound = 0;
	//slog("top to bottom:%i - %i", top_bound, bottom_bound);
	for (int i = top_bound; i <= bottom_bound; i++){
		for (int j = left_bound; j <= right_bound; j++){
			//handle tile collisions
			if (tiles[i][j] != 19&&tiles[i][j]!=10){
				collision_ctr++;
				//slog("collision check");
				handle_tile_collision(self, j, i);
				return;
			}
		}
	}
	if (collision_ctr == 0){
		//self->is_grounded = false;
		//self->r_wall_collision = false;
		//self->l_wall_collision = false;
	}
}
void handle_tile_collision(Entity *self, int x, int y){

	if (self->velocity.x>=0){
		if ((self->position.x + self->hitbox.w + self->hitbox.offsetx >= x * 16.0f && self->position.x + self->hitbox.offsetx + self->hitbox.w <= x * 16.0f + 16.0f) && (self->position.y + self->hitbox.h + self->hitbox.offsety > y * 16.0f)){
			slog("right collision");
			//self->r_wall_collision = true;
			self->position.x -= 1;
			self->velocity.x = 0;
		}
	}
	else{
		if((self->position.x + self->hitbox.offsetx >= x * 16.0f && self->position.x + self->hitbox.offsetx <= x * 16.0f + 16.0f) && (self->position.y + self->hitbox.w + self->hitbox.offsety > y * 16.0f)){
		//self->l_wall_collision = true;
		self->position.x += 1;
		self->velocity.x = 0;
		slog("left collision");
	}
	}
	//self->is_grounded = false;
	if (self->velocity.y >= 0){
		if ((self->position.y + self->hitbox.h + self->hitbox.offsety >= y * 16.0f) && (self->position.y + self->hitbox.h + self->hitbox.offsety <= (y * 16.0f+16.0f))){
			self->is_grounded = true;
			self->position.y -= 1;
			slog("top collision");
		}
	}
	else{
		 if (self->position.y + self->hitbox.offsety > y * 16 + 16){
			self->is_grounded = false;
			//self->position.y += 1;
		}
	}
}

void check_tile_ahead(Entity *self, int** tiles){
	//1 for right, 0 for left
	//check one tile ahead
	int bottom_bound = ((self->position.y + self->hitbox.offsety) / 16);//y
		int forward = ((self->position.x + self->hitbox.w + self->hitbox.offsetx) / 16)+1;//x
		if (tiles[bottom_bound][forward] != 19)
			self->r_wall_collision = true;
		else
			self->r_wall_collision = false;
		int forward_l = ((self->position.x + self->hitbox.offsetx) / 16)-1;//x
		if (tiles[bottom_bound][forward_l] != 19)
			self->l_wall_collision = true;
		else
			self->l_wall_collision = false;
}

void handle_item_collision(Entity *self, Entity *other){
	switch (other->itemType){
	case I_Health:{
					  self->health += other->health;
	}break;
	case I_Bolt:{
					self->bolts += other->bolts;
	}break;
	}
	set_to_zero_ent(other->Ent_ID);
}