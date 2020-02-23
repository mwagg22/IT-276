#include "g_collision.h"

void collision_check(Entity *ents, Uint32 entity_max){
	for (int i = 0; i < entity_max-1;i++){
		for (int j = 1; j < entity_max; j++){
			if (check_collision(&ents[i], &ents[j])){
				handle_collision(&ents[i], &ents[j]);
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
		if (other->type == ES_Projectile){
			if (other->parent->type!=ES_Player)
				handle_projectile_collision(self, other);
		}
		else if (other->type == ES_Hazard){
			handle_hazard_collision(self, other);
		}
		else if (other->type == ES_Enemy){
			self->damage(self, other->attackdmg/.5);
		}
		else if (other->type == ES_Item){
			handle_item_collision(self, other);
		}
	}break;
	}
}

bool check_hit_collision(Entity *hitlist, Entity *other){

}
void handle_hit_collision(Entity *self, Entity *other, Vector2D kick){
	self->damage(self, other->attackdmg);
}
void handle_projectile_collision(Entity *self, Entity *other){
	self->damage(self, other->attackdmg);
	if (other->proj_data.destroyOnCollision){
		set_to_zero_ent(other->Ent_ID);
	}
}
void handle_hazard_collision(Entity *self, Entity *other){

}

void check_tile_collision(Entity *self, char** tiles){
	int left_bound=(int)(self->position.x/16);//x
	int right_bound = (int)((self->position.x + self->hitbox.w) / 16);//x
	int top_bound=(int)(self->position.y/16);//y
	int bottom_bound=(int)((self->position.y+self->hitbox.h)/16);//y
	int collision_ctr = 0;
	for (int i = left_bound; i < right_bound; i++){
		for (int j = top_bound; j < left_bound; j++){
			//handle tile collisions
			if (tiles[i][j] == '1'){
				collision_ctr++;
				handle_tile_collision(self, i, j);
			}
		}
	}
	if (collision_ctr == 0){
		self->is_grounded = false;
	}
}
void handle_tile_collision(Entity *self, int x, int y){
	if (self->position.y+self->hitbox.h<y*16){
		self->is_grounded = true;
		self->position.y -= 1;
	}
	if (self->position.y>y * 16+16){
		self->position.y += 1;
	}
	if (self->position.x + self->hitbox.w > x * 16){
		self->position.x -= 1;
	}
	if (self->position.x < x * 16 + 16){
		self->position.x += 1;
	}
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