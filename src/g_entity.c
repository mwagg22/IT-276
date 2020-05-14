#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "simple_logger.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"
#include "g_camera.h"
#include "g_collision.h"
#include "g_game.h"
float framechange;
//float frame;
typedef struct
{
    Entity *entity_list;
    Uint32  entity_max;
}EntityManager;

static EntityManager gf2d_entity_manager = {0};
SDL_Event event;

void gf2d_entity_manager_close()
{
    if(gf2d_entity_manager.entity_list != NULL)
    {
        free(gf2d_entity_manager.entity_list);
    }
    memset(&gf2d_entity_manager,0,sizeof(EntityManager));
}

void gf2d_entity_manager_init(Uint32 entity_max)
{
    gf2d_entity_manager.entity_list = (Entity*)gfc_allocate_array(sizeof(Entity),entity_max);
	gf2d_entity_manager.entity_max = entity_max;
    if (!gf2d_entity_manager.entity_list)
    {
        slog("failed to allocate entity list");
        return;
    }
    atexit(gf2d_entity_manager_close);
}

Entity *gf2d_entity_new()
{
    Entity *ent = NULL;
    int i;
    for (i = 0; i < gf2d_entity_manager.entity_max; i++)
    {
        if (gf2d_entity_manager.entity_list[i]._inuse)continue;
        //. found a free entity
        memset(&gf2d_entity_manager.entity_list[i],0,sizeof(Entity));
        gf2d_entity_manager.entity_list[i]._inuse = 1;
		gf2d_entity_manager.entity_list[i].Ent_ID = (int)i;
		gf2d_entity_manager.entity_list[i].type = -1;
		gf2d_entity_manager.entity_list[i].show = true;
		slog("Entity arr :%i", i);
		//slog("Size of ents list:%i", sizeof(gf2d_entity_manager.entity_list) / sizeof(int));
        return &gf2d_entity_manager.entity_list[i];
    }
    slog("request for entity failed: all full up");
    return NULL;
}
void set_to_zero_ent(int i){
	//memset(&gf2d_entity_manager.entity_list[i], 0, sizeof(Entity));
	gf2d_entity_manager.entity_list[i]._inuse = 0;
	gf2d_entity_manager.entity_list[i].type = -1;
	
}
void gf2d_entity_free(Entity *self)
{
    if (!self)
    {
        slog("self pointer is not valid");
        return;
    }
    self->_inuse = 0;
	gf2d_sprite_free(self->sprite);
    if (self->data != NULL)
    {
        slog("warning: data not freed at entity free!");
    }
}

void gf2d_clear_entity_manager(){
	for (int i = 0; i < gf2d_entity_manager.entity_max; i++)
	{
		if (gf2d_entity_manager.entity_list[i]._inuse){
			set_to_zero_ent(i);
		}
	}
}

Entity *gf2d_return_list(){
	return gf2d_entity_manager.entity_list;
}

void draw_entities(Camera* cam){
	for (int i = 0; i < gf2d_entity_manager.entity_max; i++){
		if (gf2d_entity_manager.entity_list[i]._inuse){
			if (gf2d_entity_manager.entity_list[i].show == false)
				continue;
			if (gf2d_entity_manager.entity_list[i].type == ES_Healthbar || gf2d_entity_manager.entity_list[i].type == ES_Energybar)
				continue;
			if (return_game_controller()->pause == false){
				if (gf2d_entity_manager.entity_list[i].type == ES_Pause_Items || gf2d_entity_manager.entity_list[i].type == ES_Cursor)
					continue;
			}
			gf2d_sprite_draw(
				gf2d_entity_manager.entity_list[i].sprite,
				gf2d_entity_manager.entity_list[i].position,
				cam->position,
				NULL,
				NULL,
				NULL,
				&gf2d_entity_manager.entity_list[i].flip,
				&gf2d_entity_manager.entity_list[i].color,
				(int)gf2d_entity_manager.entity_list[i].frame);

			entity_in_bounds(&gf2d_entity_manager.entity_list[i], cam);
		}
	}
	//if (return_game_state() == G_Level)
	//should put in different function
		collision_check(gf2d_entity_manager.entity_list, gf2d_entity_manager.entity_max);
}
void update_all_entities(int type){
	//0 for all level entities
	//1 for just player
	//2 for just pause
	for (int i = 0; i < gf2d_entity_manager.entity_max; i++){
		if (gf2d_entity_manager.entity_list[i]._inuse){
			if (gf2d_entity_manager.entity_list[i].type == ES_Healthbar || gf2d_entity_manager.entity_list[i].type == ES_Energybar)
				continue;
			if (type == 0){
				;
			}
			else if (type == 1){
				if (gf2d_entity_manager.entity_list[i].type == ES_Hazard || gf2d_entity_manager.entity_list[i].type == ES_Transition || gf2d_entity_manager.entity_list[i].type == ES_Projectile || gf2d_entity_manager.entity_list[i].type == ES_Enemy || gf2d_entity_manager.entity_list[i].type == ES_Boss || (gf2d_entity_manager.entity_list[i].type == ES_BossDoor&&gf2d_entity_manager.entity_list[i].actionFrame==0))
					continue;
			}
			//pause
			else if (type == 2){
				if (gf2d_entity_manager.entity_list[i].type == ES_Player || gf2d_entity_manager.entity_list[i].type == ES_Hazard || gf2d_entity_manager.entity_list[i].type == ES_Transition || gf2d_entity_manager.entity_list[i].type == ES_Projectile || gf2d_entity_manager.entity_list[i].type == ES_Enemy || gf2d_entity_manager.entity_list[i].type == ES_Boss || gf2d_entity_manager.entity_list[i].type == ES_BossDoor)
					continue;
			}

			//update entities	
			if (gf2d_entity_manager.entity_list[i].type == ES_Pause_Items || gf2d_entity_manager.entity_list[i].type == ES_Cursor)
				update_entity(&gf2d_entity_manager.entity_list[i]);
			else if (in_camera_bounds(get_game_camera(), &gf2d_entity_manager.entity_list[i]))
				update_entity(&gf2d_entity_manager.entity_list[i]);
		}
	}
}
void update_entity(Entity *self){
		self->update_ent(self);
}

void entity_tile_collision(int** tiles){
	for (int i = 0; i < gf2d_entity_manager.entity_max; i++){
		if (gf2d_entity_manager.entity_list[i]._inuse){
			if (gf2d_entity_manager.entity_list[i].type == ES_Player&&gf2d_entity_manager.entity_list[i].clip == 0)
				continue;
			check_tile_ahead(&gf2d_entity_manager.entity_list[i], tiles);
		}
	}
}

Entity *get_player_entity(){
	for (int i = 0; i < gf2d_entity_manager.entity_max; i++){
		if (gf2d_entity_manager.entity_list[i].type==ES_Player){
			return &gf2d_entity_manager.entity_list[i];
			}
	}
	return NULL;
}
Entity *get_boss_entity(){
	for (int i = 0; i < gf2d_entity_manager.entity_max; i++){
		if (gf2d_entity_manager.entity_list[i].type == ES_Boss){
			return &gf2d_entity_manager.entity_list[i];
		}
	}
	return NULL;
}
void entity_in_bounds(Entity* self, Camera *cam){
	if (self->type == ES_Player){
		//might restart or go into death animation
		return;
	}
	if (self->type == ES_Projectile||self->type==ES_Effect){
		if (!vector_in_camera_bounds(cam, self->position)){
			set_to_zero_ent(self->Ent_ID);
			return;
		}
	}
	if (self->type == ES_Transition){
		if (!vector_in_camera_bounds(cam, self->position)){
			self->active = false;
			//slog("not active");
			return;
		}
		else{
			self->active = true;
		}
	}
	if (self->type == ES_Hazard){
		if (self->respawn == true){
			if (!vector_in_camera_bounds(cam, self->position)){
				respawn(self);
				return;
			}
		}
	}
	if (self->type == ES_BossDoor){
		if (!vector_in_camera_bounds(cam, self->position)){
			self->reset(self);
			return;
		}
	}

	if (!vector_in_camera_bounds(cam, self->start_position)){
		if (!in_camera_bounds(cam, self) || !in_camera_bounds(cam, self)&&self->state == ES_Dead){
			respawn(self);
		}
	}
}
void respawn(Entity *self){
	self->position = self->start_position;
	if (self->type == ES_Enemy || self->type == ES_Boss)
		self->reset(self);
	if (self->type == ES_Player){
		self->dir = Right;
		restart_bgm();
		reset_game_camera();
		self->reset(self);
	}
}

void set_hitbox(Entity *self, int x, int y, int w, int h,int offsetx,int offsety){
	self->hitbox.x = x;
	self->hitbox.y = y;
	self->hitbox.w = w;
	self->hitbox.h = h;
	self->hitbox.offsetx = offsetx;
	self->hitbox.offsety = offsety;
}

void update_hitbox_position(Entity *self){
	self->hitbox.x = self->position.x;
	self->hitbox.y = self->position.y;
}

void flip(Entity *self, Vector2D position){
	if (position.x > self->position.x){
		self->dir = Right;
	}
	else
	{
		self->dir = Left;
	}
}
void set_position(Entity *self, Vector2D position){
	self->position = position;
	self->start_position = position;
}
void update_entity_ent(Entity *self){
	self->frame += .1;
	if (self->frame > self->sprite->frames_per_line)
		self->frame = 0;
	
}
void init_ready_ent(Entity* self){
	self->frame = 0;
	self->color = vector4d(255, 255, 255, 255);
	self->flip = vector2d(0, 0);
	self->state = ES_Idle;
	self->update_ent = update_ready_ent;
	self->type = ES_Ready;
	self->actionFrame = 240;
	self->sprite = gf2d_sprite_load_all("../images/test/menu/ready.png", 61, 12, 1);
	self->start_position = vector2d(0, 0);
	self->position = self->start_position;
}

void update_ready_ent(Entity *self){
	self->frame += .1;
	self->position.x = get_game_camera()->position.x + get_game_camera()->viewWidth / 2-30;
	self->position.y = get_game_camera()->position.y + get_game_camera()->viewHeight / 2-10;
	if (self->frame > self->sprite->frames_per_line)
		self->frame = 0;
	if (self->actionFrame > 0)
		self->actionFrame--;
	if (self->actionFrame == 0){
		set_input_control(1);
		//set_to_zero_ent(self->Ent_ID);
		self->color = vector4d(0, 0, 0, 0);
	}
}
void init_bossdoor_ent(Entity* self,int type){
	self->frame = 0;
	self->color = vector4d(255, 255, 255, 255);
	self->flip = vector2d(0, 0);
	self->state = ES_Idle;
	self->update_ent = update_bossdoor_ent;
	self->type = ES_BossDoor;
	self->active = false;
	if (type == 1){
		self->sprite = gf2d_sprite_load_all("../images/test/transition/bossDoorUp.png", 17, 64, 4);
		self->actionFrame = 1;
	}
	else if(type==0){
		self->sprite = gf2d_sprite_load_all("../images/test/transition/bossDoorDown.png", 17, 64, 4);
		self->actionFrame = 0;
	}
	set_hitbox(self, self->position.x, self->position.y, 17, 64, 0, 0);
	self->start_position = vector2d(0, 0);
	self->position = self->start_position;
	self->reset = reset_bossdoor;
}

void update_bossdoor_ent(Entity *self){

	if (self->active&&self->actionFrame == 1 && self->frame != self->sprite->frames_per_line - 1){
		self->frame += .1;
		play_soundeffect("../sounds/bossdoor.wav", 0);
	}
	if (self->actionFrame == 0 && self->frame != self->sprite->frames_per_line - 1){
		self->frame += .1;
		play_soundeffect("../sounds/bossdoor.wav", 0);
	}
	if (self->frame > self->sprite->frames_per_line - 1){
		self->frame = self->sprite->frames_per_line-1;
	}
}
void reset_bossdoor(Entity *self){
		self->frame = 0;
		self->active = false;
}
Entity *get_bar_entity(EntityType type){
	for (int i = 0; i < gf2d_entity_manager.entity_max; i++){
		if (gf2d_entity_manager.entity_list[i].type == type){
			return &gf2d_entity_manager.entity_list[i];
		}
	}
}

void draw_ui(Camera* cam){
	for (int i = 0; i < gf2d_entity_manager.entity_max; i++){
		if (gf2d_entity_manager.entity_list[i]._inuse){
			if (gf2d_entity_manager.entity_list[i].type == ES_Healthbar || gf2d_entity_manager.entity_list[i].type == ES_Energybar || gf2d_entity_manager.entity_list[i].type == ES_Bossbar){
				update_entity(&gf2d_entity_manager.entity_list[i]);
				gf2d_sprite_draw(
					gf2d_entity_manager.entity_list[i].sprite,
					gf2d_entity_manager.entity_list[i].position,
					cam->position,
					NULL,
					NULL,
					NULL,
					NULL,
					&gf2d_entity_manager.entity_list[i].color,
					0);

				draw_bar(&gf2d_entity_manager.entity_list[i]);
			}
		}
	}
}

void draw_text(){
	for (int i = 0; i < gf2d_entity_manager.entity_max; i++){
		if (gf2d_entity_manager.entity_list[i]._inuse){
			if (gf2d_entity_manager.entity_list[i].type == ES_TextBox){
				update_entity(&gf2d_entity_manager.entity_list[i]);
				gf2d_font_draw_line(gf2d_entity_manager.entity_list[i].text_data.string, gf2d_entity_manager.entity_list[i].text_data.font, gf2d_entity_manager.entity_list[i].text_data.size, gf2d_entity_manager.entity_list[i].text_data.color, gf2d_entity_manager.entity_list[i].position);
			}
		}
	}
}
void update_ent(Entity *self){
	self->frame += .1;
	if (self->frame > self->sprite->frames_per_line)
		self->frame = 0;
}
void init_transition_ent(Entity* self, Vector2D topleft,Vector2D bottomright, Dir dir){
	self->frame = 0;
	self->color = vector4d(255, 255, 255, 0);
	self->flip = vector2d(0, 0);
	self->state = ES_Idle;
	self->update_ent = update_ent;
	self->type = ES_Transition;
	self->actionFrame = 240;
	self->dir = dir;
	self->transition_dir = dir;
	self->sprite = gf2d_sprite_load_all("../images/test/transition/box.png", 16,16, 1);
	set_hitbox(self, self->position.x, self->position.y, 16, 16, 0, 0);
	self->start_position = vector2d(0, 0);
	self->position = self->start_position;
	self->topbounds = topleft;
	self->bottombounds = bottomright;
}

void init_color_ent(Entity* self){
	self->frame = 0;
	self->color = return_game_controller()->data->coloredit;
	self->flip = vector2d(0, 0);
	self->state = ES_Idle;
	self->update_ent = update_color_ent;
	self->sprite = gf2d_sprite_load_all("../images/test/idle.png", 40, 40, 1);
	set_hitbox(self, self->position.x, self->position.y, 0, 0, 0, 0);
	self->start_position = vector2d(110, 92);
	self->position = self->start_position;
}
void update_color_ent(Entity* self){
	self->color = return_game_controller()->data->coloredit;
}
/*eol@eof*/