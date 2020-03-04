#include "g_collision.h"
#include "simple_logger.h"
#include <math.h>

void collision_check(Entity *ents, Uint32 entity_max){
	for (int i = 0; i < entity_max-1;i++){
		if (ents[i].state == ES_Dead)
			continue;
		for (int j = 1; j < entity_max; j++){
			if (ents[j].state == ES_Dead)
				continue;
			if (check_collision(&ents[i], &ents[j])){
				//slog("collision");
				handle_collision(&ents[i], &ents[j]);
				handle_collision(&ents[j], &ents[i]);
			}
		}
	}
}

bool check_collision(Entity *self, Entity *other){
	return(
		self->position.x + self->hitbox.w + self->hitbox.offsetx > other->position.x + other->hitbox.offsetx &&
		self->position.x + self->hitbox.offsetx  < other->position.x + other->hitbox.offsetx + other->hitbox.w &&
		self->position.y + self->hitbox.h + self->hitbox.offsety > other->position.y + other->hitbox.offsety &&
		self->position.y + self->hitbox.offsety < other->position.y + other->hitbox.offsety + other->hitbox.h);
}

void handle_collision(Entity *self, Entity *other){
	//do stuff
	switch (self->type){
	case ES_Player:{
	   int dir = (other->position.x >= self->position.x) ? -1 : 1;
	    Vector2D kick = vector2d(5 * dir, 0);
		if (self->invincibleFrame == 0){
			if (other->type == ES_Projectile){
				if (other->proj_data.parentType != ES_Player)
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
		int dir = (other->position.x >= self->position.x) ? -1 : 1;
		Vector2D kick = vector2d(5 * dir, 0);
			if (other->type == ES_Projectile){
				//damage
					if (other->proj_data.parentType != ES_Enemy)
						handle_projectile_collision(self, other, kick);
					}
			else if (other->type == ES_Hazard){
							 //die instant
					}
	}break;
	case ES_Projectile:{
						   if (other->type == ES_Player){
							   //destroy self
							   if (self->proj_data.parentType != ES_Player){
								   handle_projectile_collision(self, other, vector2d(0, 0));
							   }
						   }
						   if (other->type == ES_Enemy){
							   //destroy self
							   if (self->proj_data.parentType != ES_Enemy){
								   handle_projectile_collision(self, other, vector2d(0, 0));
							   }
						   }
	}break;
	case ES_Item:{
						   if (other->type == ES_Player){
							   //destroy self
								  handle_item_collision(self, other);
						   }
	}break;
	}
}

bool check_hit_collision(Entity *hitlist, Entity *other){

}
void handle_hit_collision(Entity *self, Entity *other, Vector2D kick){

}

void handle_projectile_collision(Entity *self, Entity *other,Vector2D kick){
	if (self->type == ES_Projectile){
		if (self->proj_data.destroyOnCollision){
			set_to_zero_ent(self->Ent_ID);
		}
	}
	else
		self->damage(self, other->attackdmg, kick);
}

void handle_hazard_collision(Entity *self, Entity *other, Vector2D kick){
	if (other->can_attack)
		self->damage(self, other->attackdmg, kick);
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
	for (int i = left_bound; i <= right_bound; i++){
		for (int j = top_bound; j <= bottom_bound; j++){
			//handle tile collisions
			if (tiles[j][i] != 19&&tiles[j][i]!=10){
				collision_ctr++;
				//slog("collision check");
				//handle_tile_collision(self, i, j);
				//return;
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

	//if (self->velocity.x>=0){
		if ((self->position.x + self->hitbox.w + self->hitbox.offsetx > x * 16.0f && self->position.x + self->hitbox.offsetx + self->hitbox.w < x * 16.0f + 16.0f) && (self->position.y + self->hitbox.h + self->hitbox.offsety > y * 16.0f)){
			//slog("right collision");
			//self->r_wall_collision = true;
			self->position.x -= 1;
			self->velocity.x = 0;
		}
	//}
	//else{
		if((self->position.x + self->hitbox.offsetx > x * 16.0f && self->position.x + self->hitbox.offsetx < x * 16.0f + 16.0f) && (self->position.y + self->hitbox.w + self->hitbox.offsety > y * 16.0f)){
		//self->l_wall_collision = true;
		self->position.x += 1;
		self->velocity.x = 0;
		//slog("left collision");
	//}
	}
	//self->is_grounded = false;
	if (self->velocity.y >= 0){
		if ((self->position.y + self->hitbox.h + self->hitbox.offsety > y * 16.0f)){
			//self->is_grounded = true;
			//self->position.y -= 1;

			//slog("top collision");
		}
	}
	else{
		 if (self->position.y + self->hitbox.offsety > y * 16 + 16){
			//self->is_grounded = false;
			//self->position.y += 1;
		}
	}
}


void check_tile_ahead(Entity* self, int** tiles){
	//left tile
	int upper_bound = ((self->position.y + self->hitbox.offsety) / 16);
	int lower_bound = ((self->position.y + self->hitbox.offsety + self->hitbox.h) / 16);
	int left_tile = ((self->position.x + self->hitbox.offsetx) / 16) - 1;
	int l = 0;
	int r = 0;
	//lower tile

	int left_bound = ((self->position.x + self->hitbox.offsetx) / 16);
	int right_bound = ((self->position.x + self->hitbox.w + self->hitbox.offsetx) / 16);
	int bottom_tile = ((self->position.y + self->hitbox.offsety + self->hitbox.h) / 16);
	int t = 0;
	for (int i = left_bound; i <= right_bound; i++){
		if (tiles[bottom_tile][i] != 19 && vector2d_magnitude_between(vector2d(0, self->position.y + self->hitbox.offsety + self->hitbox.h), vector2d(0, bottom_tile * 16)) <= 1){
			self->is_grounded = true;
			t++;
			//slog("grownded");
			//return;
		}
	}
	if (t == 0){
		if (!self->jumped){
			self->falling = true;
		}
		self->is_grounded = false;
	}
	for (int i = upper_bound; i <= lower_bound; i++){
		if (self->is_grounded&&i == lower_bound)
			continue;
		if (tiles[i][left_tile] != 19 && vector2d_magnitude_between(vector2d(self->position.x + self->hitbox.offsetx, 0), vector2d(left_tile * 16 + 16, 0)) <= 1 ){
			self->l_wall_collision = true;
			l++;
			//slog("left wall collision");
		}
	}
	if (l == 0){
		self->l_wall_collision = false;
	}
	//right tile
	int right_tile = ((self->position.x + self->hitbox.w + self->hitbox.offsetx) / 16) + 1;
	for (int i = upper_bound; i <= lower_bound; i++){
		if (self->is_grounded&&i == lower_bound)
			continue;
		if (tiles[i][right_tile] != 19 && vector2d_magnitude_between(vector2d(self->position.x + self->hitbox.w + self->hitbox.offsetx, 0), vector2d(right_tile * 16, 0)) <= 1){
			self->r_wall_collision = true;
			//return;
			r++;
			//slog("right wall collision");
			
		}
		
		//if (tiles[i][right_tile - 1] != 19){
			//self->position.x -=1;
		//}

	}
	if (r == 0){
		self->r_wall_collision = false;
	}

	
}


void handle_item_collision(Entity *self, Entity *other){
	if (self->type == ES_Player){
		switch (other->itemType){
		case I_Health:{
						  self->health += other->health;
		}break;
		case I_Bolt:{
						self->bolts += other->health;
		}break;
		case I_Energy:{
						  self->energy += other->health;
		}break;
		}
	}
	else{
		set_to_zero_ent(self->Ent_ID);
	}
}