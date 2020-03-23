#include "g_bar.h"
#include "g_game.h"

void update_bar_ent(Entity *self){
	switch (self->type){
	case(ES_Healthbar) : {
							 self->position.x = get_game_camera()->position.x + 10.0;
							 self->position.y = get_game_camera()->position.y + 4.0;
	}break;
	case(ES_Energybar) : {
							 self->position.x = get_game_camera()->position.x + 20.0;
							 self->position.y = get_game_camera()->position.y + 4.0;
							 if (self->parent->weapons_list.currentWeapon==0)
								self->color = vector4d(255, 255, 255, 0);
							 else
								 self->color = vector4d(255, 255, 255, 255);
	}break;
	case(ES_Bossbar) : {
						   self->position.x = get_game_camera()->position.x + 240;
						   self->position.y = get_game_camera()->position.y + 4;
						   if (!in_camera_bounds(get_game_camera(), get_boss_entity())){
							   self->_inuse = 0;
						   }
	}break;
	}
}

void init_bar_ent(Entity *self, EntityType type){
	self->frame = 0;
	self->clip = 0;
	self->type = type;
	self->color = vector4d(255, 255, 255, 255);
	self->start_position = vector2d(0, 0);
	self->position = self->start_position;
	self->update_ent = update_bar_ent;
	self->sprite = gf2d_sprite_load_all("../images/test/bar/bar.png", 8, 56, 1);
	switch (type){
	case(ES_Healthbar) : {	self->parent = get_player_entity();
	}break;
	case(ES_Energybar) : {	self->parent = get_player_entity();
	}break;
	case(ES_Bossbar) : {	self->parent = get_boss_entity();
	}break;
	}
}
void create_bar_ent(EntityType type){
	Entity* self = gf2d_entity_new();
	init_bar_ent(self, type);
}
void draw_bar(Entity* self){
	switch (self->type){
	case(ES_Healthbar) : {
							 Sprite* amount = gf2d_sprite_load_all("../images/test/bar/amount.png", 6, 1, 1);
							 int ctr = 2;
							 Vector2D position;
							 for (int i = 0; i < self->parent->health; i++){
								 position = vector2d(self->position.x + 1, self->position.y + 56 - ctr);
								 gf2d_sprite_draw(amount,position , get_game_camera()->position, NULL, NULL, NULL, NULL, &self->color, 0);
								 //slog("drawing health");
								 ctr += 2;
							 }
	}break;
	case(ES_Bossbar) : {
							 Sprite* amount = gf2d_sprite_load_all("../images/test/bar/amount.png", 6, 1, 1);
							 int ctr = 2;
							 Vector2D position;
							 Vector4D color = vector4d(255, 0, 0, 255);
							 for (int i = 0; i < self->parent->health; i++){
								 position = vector2d(self->position.x + 1, self->position.y + 56 - ctr);
								 gf2d_sprite_draw(amount, position, get_game_camera()->position, NULL, NULL, NULL, NULL, &color, 0);
								 //slog("drawing health");
								 ctr += 2;
							 }
	}break;
	case(ES_Energybar) : {
							 Sprite* amount = gf2d_sprite_load_all("../images/test/bar/amount.png", 6, 1, 1);
							 int ctr = 2;
							 Vector2D position;
							 for (int i = 2; i <= self->parent->energy; i++){
								 //slog("drawing energy");
								 position = vector2d(self->position.x + 1, self->position.y + 56 - ctr);
								 gf2d_sprite_draw(amount, position, get_game_camera()->position, NULL, NULL, NULL, NULL, &self->color, 0);
								 ctr += 2;
							 }
	}break;
	}
}