#include "g_item.h"
#include "g_game.h"


void update_text_ent(Entity *self){
	if(self->text_data.follow){
		self->position.x = get_game_camera()->position.x + self->position.x;
		self->position.y = get_game_camera()->position.y + self->position.y;
	}
	self->text_data.aliveFrame--;
	if (self->text_data.aliveFrame == 0){
		//destroy self
	}
}

void init_text(Entity *self, int aliveframe, int size, char* string,char* font, Vector4D color, Vector2D position, bool follow){
	self->text_data.aliveFrame=aliveframe;
	self->text_data.size=size;
	self->text_data.string=string;
	self->position=position;
	self->start_position = position;
	set_hitbox(self, self->position.x, self->position.y, 0, 0, 0, 0);
	self->color = vector4d(255,255,255,0);
	self->text_data.follow = follow;
	self->text_data.font = font;
	self->frame = 0;
	self->clip = 0;
	self->text_data.aliveFrame = aliveframe;
	self->type = ES_TextBox;
	self->update_ent = update_text_ent;
}

void text_set_position(Entity *self, Vector2D position){
	self->position = position;
}

void update_text(Entity *self, int aliveframe, int size, char* string, Vector4D color, Vector2D position, bool follow){
	self->text_data.aliveFrame = aliveframe;
	self->text_data.size = size;
	self->text_data.string = string;
	self->text_data.color = color;
	self->text_data.follow = follow;
}