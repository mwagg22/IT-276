#include "gf2d_sprite.h"
#include "g_entity.h"

#ifndef menu_s
#define menu_s

typedef struct main_menu_s{
	Entity* cursor;
	Entity* title;
	Entity* menu_items;
	int select;
}main_menu;

typedef struct select_screen_s{
	Entity* cursor;
	Entity** icon_items;
	int select;
}select_screen;


void update_menu_ent(Entity *self);
main_menu* init_menu_ent();
select_screen* init_select_screen();
void init_item(Entity* self, int item);
void get_menu_inputs(main_menu* menu,const Uint8* keys);
void get_select_inputs(select_screen* select, const Uint8* keys);
void menu_set_position(Entity *self, Vector2D position);
void create_menu(Item_type type, int amount, int aliveFrame, Vector2D position);

#endif