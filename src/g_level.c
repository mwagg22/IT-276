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
}

bool check_hit_collision(Entity *hitlist, Entity *other){

}
void handle_hit_collision(Entity *self, Entity *other, Vector2D kick){

}
void handle_projectile_collision(Entity *self, Entity *other){

}
