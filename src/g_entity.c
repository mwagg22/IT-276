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
	gf2d_entity_manager.entity_list[i]._inuse = 0;
	memset(&gf2d_entity_manager.entity_list[i], 0, sizeof(Entity));
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

			//slog("cam position %f,%f", cam->position.x, cam->position.y);
			//check within bounds
			//update entities	
			update_entity(&gf2d_entity_manager.entity_list[i]);
			//if (return_game_state()==G_Level)
				entity_in_bounds(&gf2d_entity_manager.entity_list[i], cam);
		}
	}
	//if (return_game_state() == G_Level)
		collision_check(gf2d_entity_manager.entity_list, gf2d_entity_manager.entity_max);
}

void update_entity(Entity *self){
		self->update_ent(self);
}

void entity_tile_collision(int** tiles){
	for (int i = 0; i < gf2d_entity_manager.entity_max; i++){
		if (gf2d_entity_manager.entity_list[i]._inuse){
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
	if (self->type == ES_Hazard){
		if (!vector_in_camera_bounds(cam, self->position)){
			respawn(self);
			return;
		}
	}
	if (!vector_in_camera_bounds(cam, self->start_position)){
		if (!in_camera_bounds(cam, self)||self->state==ES_Dead){
			respawn(self);
		}
	}
}
void respawn(Entity *self){
	self->position = self->start_position;
	self->health = self->healthmax;
	self->state = ES_Idle;
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
/*eol@eof*/