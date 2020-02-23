#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "simple_logger.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"
#include "g_camera.h"
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
			//slog("position at i:%f,%f", gf2d_entity_manager.entity_list[i].position.x, gf2d_entity_manager.entity_list[i].position.y);
		}
	}
}

Entity *get_player_entity(){
	for (int i = 0; i < gf2d_entity_manager.entity_max; i++){
		if (gf2d_entity_manager.entity_list[i]._inuse&&gf2d_entity_manager.entity_list[i].type==ES_Player){
			return &gf2d_entity_manager.entity_list[i];
			}
	}
}

void respawn(Entity *self){
	self->position = self->start_position;
}
/*eol@eof*/