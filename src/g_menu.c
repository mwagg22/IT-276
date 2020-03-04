#include "g_menu.h"
#include "g_game.h"
SDL_Event event;

main_menu* init_menu_ent(){
	main_menu* menu = (main_menu*)malloc(sizeof(main_menu));
	menu->menu_items = (Entity*)malloc(sizeof(Entity)* 2);
	Entity *title = gf2d_entity_new();
	init_item(title, 0);

	Entity *cursor = gf2d_entity_new();
	init_item(cursor, 1);

	Entity *start = gf2d_entity_new();
	init_item(start, 2);

	Entity *save = gf2d_entity_new();
	init_item(save, 3);

	menu->menu_items[0] = *start;
	menu->menu_items[1] = *start;
	menu->cursor = cursor;
	menu->select = 0;
	
	return menu;
}

select_screen* init_select_screen(){
	select_screen* select = (select_screen*)malloc(sizeof(select_screen));
	select->icon_items = (Entity*)malloc(sizeof(Entity)* 4);

	Entity *cursor = gf2d_entity_new();
	init_item(cursor, 1);

	Entity *start = gf2d_entity_new();
	init_item(start, 2);

	Entity *save = gf2d_entity_new();
	init_item(save, 3);

	menu->menu_items[0] = *start;
	menu->menu_items[1] = *start;
	select->cursor = cursor;
	select->select = 0;

	return select;
}
void init_item(Entity* self, int item){
	self->frame = 0;
	self->color = vector4d(255, 255, 255, 255);
	self->state = ES_Idle;
	self->update_ent = update_menu_ent;
	self->type = ES_Menu;
	switch (item){
	case(0):{
		self->sprite_list.idle = gf2d_sprite_load_all("../images/test/menu/title.png", 182, 55, 1);
		self->start_position = vector2d(35, 54);
		self->position = self->start_position;
		}break;
	case(1):{
		self->sprite_list.idle = gf2d_sprite_load_all("../images/test/menu/cursor.png", 182, 55, 1);
		self->start_position = vector2d(36, 160);
		self->position = self->start_position;
	}break;
	case(2) : {
				  self->sprite_list.idle = gf2d_sprite_load_all("../images/test/menu/gamestart.png", 182, 55, 1);
				  self->start_position = vector2d(54, 160);
				  self->position = self->start_position;
	}break;
	case(3) : {
				  self->sprite_list.idle = gf2d_sprite_load_all("../images/test/menu/continue.png", 182, 55, 1);
				  self->start_position = vector2d(54, 180);
				  self->position = self->start_position;
	}break;
	}
}

void get_menu_inputs(main_menu* menu, const Uint8* keys){
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym){
			case SDLK_UP: {
							  menu->select--;
							  if (menu->select < 0)
								  menu->select = 0;
			}
				break;
			case SDLK_DOWN:{
							   menu->select++;
							   if (menu->select>1)
								   menu->select = 1;
			}break;
			case SDLK_a:{
							if (menu->select==0)
								set_game_state(G_BossSelect);
							else{
								//load save file
							}
			}
			}break;
		}
	}
	

	set_position(menu->cursor, vector2d(menu->cursor->position.x, menu->menu_items[menu->select].position.y));
	
}

void update_menu_ent(Entity *self){
	self->frame += .1;
}
