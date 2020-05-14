#include <stdlib.h>
#include <stdio.h>
#include "g_menu.h"
#include "g_game.h"
#include "g_text.h"

SDL_Event event;

main_menu* init_menu_ent(){
	main_menu* menu = (main_menu*)malloc(sizeof(main_menu));
	menu->menu_items = (Entity*)malloc(sizeof(Entity)* 3);
	Entity *title = gf2d_entity_new();
	init_item(title, 0);

	Entity *cursor = gf2d_entity_new();
	init_item(cursor, 1);

	Entity *start = gf2d_entity_new();
	init_item(start, 2);

	Entity *save = gf2d_entity_new();
	init_item(save, 3);

	Entity *color = gf2d_entity_new();
	init_item(color, 22);

	menu->menu_items[0] = *start;
	menu->menu_items[1] = *save;
	menu->menu_items[2] = *color;
	menu->cursor = cursor;
	menu->select = 0;
	
	return menu;
}

select_screen* init_select_screen(){
	select_screen* select = (select_screen*)malloc(sizeof(select_screen));
	select->icon_items = (Entity**)gfc_allocate_array(sizeof(Entity*), 4);
	select->selected = false;
	Entity *background = gf2d_entity_new();
	init_item(background, 9);

	Entity *fireman = gf2d_entity_new();
	init_item(fireman, 5);

	Entity *test = gf2d_entity_new();
	init_item(test, 6);

	Entity *metalman = gf2d_entity_new();
	init_item(metalman, 7);

	Entity *crashman = gf2d_entity_new();
	init_item(crashman, 8);

	Entity *cursor = gf2d_entity_new();
	init_item(cursor, 4);

	select->icon_items[0] = fireman;
	
	select->icon_items[1] = test;
	select->icon_items[2] = metalman;
	select->icon_items[3] = crashman;

	select->cursor = cursor;
	select->select = 0;
	slog("position %f %f", fireman->position.x, fireman->position.y);
	return select;
}

pause_menu* init_pause_screen(){
	pause_menu* menu = (pause_menu*)malloc(sizeof(pause_menu));
	menu->menu_items = (Entity*)malloc(sizeof(Entity)* 2);
	
	Entity *background2 = gf2d_entity_new();
	init_item(background2, 14);

	Entity *background = gf2d_entity_new();
	init_item(background, 11);

	

	Entity *cursor = gf2d_entity_new();
	init_item(cursor, 10);

	Entity *contin = gf2d_entity_new();
	init_item(contin, 12);

	Entity *edit = gf2d_entity_new();
	init_item(edit, 18);

	Entity *quit = gf2d_entity_new();
	init_item(quit, 13);

	menu->menu_items[0] = contin;
	menu->menu_items[1] = edit;
	menu->menu_items[2] = quit;
	menu->cursor = cursor;
	menu->select = 0;
	menu->active = 0;

	return menu;
}

bullet_menu* init_bulletmenu_screen(){
	bullet_menu* menu = (bullet_menu*)malloc(sizeof(bullet_menu));
	menu->menu_items = (Entity*)malloc(sizeof(Entity)* 5);

	Entity *background = gf2d_entity_new();
	init_item(background, 15);

	Entity *item1 = gf2d_entity_new();
	init_item(item1, 17);

	Entity *item2 = gf2d_entity_new();
	init_item(item2, 17);
	item2->start_position = vector2d(194, 50);
	item2->position = vector2d(194, 50);

	Entity *item3 = gf2d_entity_new();
	init_item(item3, 19);

	Entity *item4 = gf2d_entity_new();
	init_item(item4, 20);

	Entity *item5 = gf2d_entity_new();
	init_item(item5, 17);
	item5->start_position = vector2d(194, 170);
	item5->position = vector2d(194, 170);

	Entity *cursor = gf2d_entity_new();
	init_item(cursor, 16);

	menu->background = background;
	menu->menu_items[0] = item1;
	menu->menu_items[1] = item2;
	menu->menu_items[2] = item3;
	menu->menu_items[3] = item4;
	menu->menu_items[4] = item5;
	menu->cursor = cursor;
	menu->select = 0;
	menu->active = 0;

	return menu;
}

color_menu* init_colormenu_screen(){
	color_menu* menu = (color_menu*)malloc(sizeof(color_menu));
	menu->menu_items = (Entity*)malloc(sizeof(Entity)* 3);
	menu->data = (int*)malloc(sizeof(int)* 3);
	menu->data[0] = return_game_controller()->data->coloredit.x;
	menu->data[1] = return_game_controller()->data->coloredit.y;
	menu->data[2] = return_game_controller()->data->coloredit.z;

	char* str = malloc(10);
	char* str2 = malloc(10);
	char* str3 = malloc(10);
	sprintf(str, "%d", menu->data[0]);
	sprintf(str2, "%d", menu->data[1]);
	sprintf(str3, "%d", menu->data[2]);

	Entity *background = gf2d_entity_new();
	init_item(background, 23);

	Entity *character = gf2d_entity_new();
	init_color_ent(character);

	Entity *red = gf2d_entity_new();
	init_text(red, -1, 18, str, "../font/VTSR____.ttf", vector4d(255, 255, 255, 255), vector2d(184,135), false);

	
	Entity *blue = gf2d_entity_new();
	init_text(blue, -1, 18, str2, "../font/VTSR____.ttf", vector4d(255, 255, 255, 255), vector2d(184, 165), false);

	
	Entity *green = gf2d_entity_new();
	init_text(green, -1, 18, str3, "../font/VTSR____.ttf", vector4d(255, 255, 255, 255), vector2d(184, 195), false);

	Entity *cursor = gf2d_entity_new();
	init_item(cursor, 21);

	menu->menu_items[0] = red;
	menu->menu_items[1] = blue;
	menu->menu_items[2] = green;
	menu->cursor = cursor;
	menu->select = 0;

	return menu;

}
void init_item(Entity* self, int item){
	self->frame = 0;
	self->color = vector4d(255, 255, 255, 255);
	self->flip = vector2d(0, 0);
	self->state = ES_Idle;
	self->update_ent = update_menu_ent;
	self->type = ES_Menu;
	switch (item){
		//0-3 main menu 4-8 select screen 9 select background
	case(0):{
		self->sprite = gf2d_sprite_load_all("../images/test/menu/title.png", 182, 55, 1);
		self->start_position = vector2d(35, 54);
		self->position = self->start_position;
		}break;
	case(1):{
				self->sprite = gf2d_sprite_load_all("../images/test/menu/cursor.png", 11, 11, 1);
				self->start_position = vector2d(36, 160);
				self->position = self->start_position;
	}break;
	case(2) : {
				  self->sprite = gf2d_sprite_load_all("../images/test/menu/gamestart.png", 88, 9, 1);
				  self->start_position = vector2d(54, 160);
				  self->position = self->start_position;
	}break;
	case(3) : {
				  self->sprite = gf2d_sprite_load_all("../images/test/menu/continue.png", 88, 9, 1);
				  self->start_position = vector2d(54, 180);
				  self->position = self->start_position;
	}break;
	case(4) : {
				  self->sprite = gf2d_sprite_load_all("../images/test/menu/stageselectcursor.png", 45, 45, 2);
				  self->start_position = vector2d(53, 46);
				  self->position = self->start_position;
	}break;
	case(5) : {
				  self->sprite = gf2d_sprite_load_all("../images/test/menu/firemanstage.png", 45, 45, 2);
				  self->start_position = vector2d(53, 46);
				  self->position = self->start_position;
				  self->type = ES_Icon;
	}break;
	case(6) : {
				  self->sprite = gf2d_sprite_load_all("../images/test/menu/teststage.png", 45, 45, 2);
				  self->start_position = vector2d(157, 46);
				  self->position = self->start_position;
				  self->type = ES_Icon;
	}break;
	case(7) : {
				  self->sprite = gf2d_sprite_load_all("../images/test/menu/metalmanstage.png", 45, 45, 2);
				  self->start_position = vector2d(53, 140);
				  self->position = self->start_position;
				  self->type = ES_Icon;
	}break;
	case(8) : {
				  self->sprite = gf2d_sprite_load_all("../images/test/menu/crashmanstage.png", 45, 45, 2);
				  self->start_position = vector2d(157, 140);
				  self->position = self->start_position;
				  self->type = ES_Icon;
	}break;
	case(9) : {
				  self->sprite = gf2d_sprite_load_all("../images/test/menu/selectscreen.png", 256, 240, 1);
				  self->start_position = vector2d(0, 0);
				  self->position = self->start_position;
	}break;
	case(10) : {
				   self->sprite = gf2d_sprite_load_all("../images/test/menu/cursor.png", 11, 11, 1);
				   self->start_position = vector2d(30, 89);
				   self->position = self->start_position;
				   self->type = ES_Cursor;
	}break;
	case(11) : {
				  self->sprite = gf2d_sprite_load_all("../images/test/menu/pausescreen.png", 252, 136, 1);
				  self->start_position = vector2d(1, 52);
				  self->position = self->start_position;
				  self->type = ES_Pause_Items;
	}break;
	case(12) : {
				   self->sprite = gf2d_sprite_load_all("../images/test/menu/pause_continue.png", 86, 14, 1);
				   self->start_position = vector2d(42, 89);
				   self->position = self->start_position;
				   self->type = ES_Pause_Items;
	}break;
	case(13) : {
				   self->sprite = gf2d_sprite_load_all("../images/test/menu/pause_quit.png", 41, 16, 1);
				   self->start_position = vector2d(42, 131);
				   self->position = self->start_position;
				   self->type = ES_Pause_Items;
	}break;
	case(14) : {
				   self->sprite = gf2d_sprite_load_all("../images/test/menu/pause_background.png", 256, 240, 1);
				   self->start_position = vector2d(0, 0);
				   self->position = self->start_position;
				   self->type = ES_Pause_Items;
	}break;
	case(15) : {
				   self->sprite = gf2d_sprite_load_all("../images/test/menu/edit/menu.png", 256, 240, 1);
				   self->start_position = vector2d(0, 0);
				   self->position = self->start_position;
				   self->type = ES_Pause_Items;
				   self->show = false;
	}break;
	case(16) : {
				   self->sprite = gf2d_sprite_load_all("../images/test/menu/edit/select.png", 59, 26, 1);
				   self->color = vector4d(255, 255, 255, 100);
				   self->start_position = vector2d(194, 24);
				   self->position = self->start_position;
				   self->type = ES_Cursor;
				   self->show = false;
	}break;
	case(17) : {
				   self->sprite = gf2d_sprite_load_all("../images/test/menu/edit/options.png", 59, 26, 3);
				   self->start_position = vector2d(194, 24);
				   self->position = self->start_position;
				   self->type = ES_Pause_Items;
				   self->show = false;
	}break;
	case(18) : {
				   self->sprite = gf2d_sprite_load_all("../images/test/menu/pause_customize.png", 97, 14, 1);
				   self->start_position = vector2d(42, 110);
				   self->position = self->start_position;
				   self->type = ES_Pause_Items;
	}break;
	case(19) : {
				   self->sprite = gf2d_sprite_load_all("../images/test/menu/edit/options_angles.png", 59, 26, 3);
				   self->start_position = vector2d(194, 76);
				   self->position = self->start_position;
				   self->type = ES_Pause_Items;
				   self->show = false;
	}break;
	case(20) : {
				   self->sprite = gf2d_sprite_load_all("../images/test/menu/edit/options_yn.png", 59, 26, 2);
				   self->start_position = vector2d(194, 145);
				   self->position = self->start_position;
				   self->type = ES_Pause_Items;
				   self->show = false;
	}break;
	case(21) : {
				   self->sprite = gf2d_sprite_load_all("../images/test/menu/edit/color_select.png", 68, 23, 1);
				   self->color = vector4d(255, 255, 255, 100);
				   self->start_position = vector2d(167, 130);
				   self->position = self->start_position;
				   //self->type = ES_Cursor;
	}break;
	case(22) : {
				  self->sprite = gf2d_sprite_load_all("../images/test/menu/coloredit.png", 88, 9, 1);
				  self->start_position = vector2d(54, 200);
				  self->position = self->start_position;
	}break;
	case(23) : {
				   self->sprite = gf2d_sprite_load_all("../images/test/menu/edit/color_screen.png", 256, 240, 1);
				   self->start_position = vector2d(0, 0);
				   self->position = self->start_position;
	}break;
	default:{
				;
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
							  menu_set_position(menu->cursor, vector2d(menu->cursor->position.x, menu->menu_items[menu->select].position.y));
			}
				break;
			case SDLK_DOWN:{
							   menu->select++;
							   if (menu->select>2)
								   menu->select = 2;
							   menu_set_position(menu->cursor, vector2d(menu->cursor->position.x, menu->menu_items[menu->select].position.y));
						
			}break;
			case SDLK_a:{
							if (menu->select == 0){
								play_soundeffect("../sounds/warp.wav", 0);
								set_game_state(G_BossSelect, 0);
							}
							else if (menu->select == 1){
								//load save file
								play_soundeffect("../sounds/warp.wav", 0);
								load_save();
							}
							else{
								play_soundeffect("../sounds/warp.wav", 0);
								set_game_state(G_ColorEdit, 0);
							}
			}
			}break;
		}
	}
	//
}

void get_pause_inputs(pause_menu* menu, const Uint8* keys){
	if (menu->active == 0){
		menu->active = 1;
		menu_set_position(menu->cursor, vector2d(menu->menu_items[menu->select]->position.x-15, menu->menu_items[menu->select]->position.y));
		slog("set");
	}
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym){
			case SDLK_UP: {
							  play_soundeffect("../sounds/menuselect.wav", 0);
							  menu->select--;
							  if (menu->select < 0)
								  menu->select = 0;
							  menu_set_position(menu->cursor, vector2d(menu->cursor->position.x, menu->menu_items[menu->select]->position.y));
			}
				break;
			case SDLK_DOWN:{
							   play_soundeffect("../sounds/menuselect.wav", 0);
							   menu->select++;
							   if (menu->select>2)
								   menu->select = 2;
							   menu_set_position(menu->cursor, vector2d(menu->cursor->position.x, menu->menu_items[menu->select]->position.y));

			}break;
			case SDLK_a:{
							if (menu->select == 0){
								return_game_controller()->pause = false;
								set_update_type(0);
								menu->active = 0;
							}	
							else if (menu->select == 1){
								return_game_controller()->bullet_edit = true;
								show_bulletmenu(return_game_controller()->bulletMenu);
							}
							else{
								//return to selection screen
								return_game_controller()->pause = false;
								set_update_type(0);
								set_game_state(G_BossSelect, 0);
							}
			}
			}break;
		}
	}
	//
}

void get_select_inputs(select_screen* select, const Uint8* keys){
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym){
			case SDLK_UP: {
							  play_soundeffect("../sounds/menuselect.wav", 0);
							  select->select-=2;
							  if (select->select < 0)
								  select->select += 2;
							  menu_set_position(select->cursor, vector2d(select->icon_items[select->select]->position.x, select->icon_items[select->select]->position.y));
			}
				break;
			case SDLK_DOWN:{
							   play_soundeffect("../sounds/menuselect.wav", 0);
							   select->select+=2;
							   if (select->select>3)
								   select->select -=2 ;
							   menu_set_position(select->cursor, vector2d(select->icon_items[select->select]->position.x, select->icon_items[select->select]->position.y));
			}break;
			case SDLK_LEFT: {
							  play_soundeffect("../sounds/menuselect.wav", 0);
							  select->select -= 1;
							  if (select->select < 0)
								  select->select += 1;
							  menu_set_position(select->cursor, vector2d(select->icon_items[select->select]->position.x, select->icon_items[select->select]->position.y));
			}
				break;
			case SDLK_RIGHT:{
								play_soundeffect("../sounds/menuselect.wav", 0);
							   select->select +=1;
							   if (select->select>3)
								   select->select -= 1;
							   menu_set_position(select->cursor, vector2d(select->icon_items[select->select]->position.x, select->icon_items[select->select]->position.y));

			}break;
			case SDLK_a:{
							if (!select->selected){
								select->selected = true;
								play_soundeffect("../sounds/warp.wav", 0);
								set_game_state(G_Level, select->select);

							}
								
			}break;
			case SDLK_s:{
							if (!select->selected){
								select->selected = true;
								set_game_state(G_Menu, 0);

							}

			}break;
			}break;
		}
	}
	
	//slog("position %f %f", select->icon_items[select->select]->position.x, select->icon_items[select->select]->position.y);
}

void get_bulletmenu_inputs(bullet_menu* menu, const Uint8* keys){
	if (menu->active == 0){
		menu->active = 1;
		menu_set_position(menu->cursor, vector2d(menu->menu_items[menu->select]->position.x, menu->menu_items[menu->select]->position.y));
		slog("set");
	}
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym){
			case SDLK_UP: {
							  play_soundeffect("../sounds/menuselect.wav", 0);
							  menu->select--;
							  if (menu->select < 0)
								  menu->select = 0;
							  menu_set_position(menu->cursor, vector2d(menu->cursor->position.x, menu->menu_items[menu->select]->position.y));
			}
				break;
			case SDLK_DOWN:{
							   play_soundeffect("../sounds/menuselect.wav", 0);
							   menu->select++;
							   if (menu->select>4)
								   menu->select = 4;
							   menu_set_position(menu->cursor, vector2d(menu->cursor->position.x, menu->menu_items[menu->select]->position.y));

			}break;
			case SDLK_a:{
							int number = menu->menu_items[menu->select]->frame + 1;
							int max_sprite_index = menu->menu_items[menu->select]->sprite->frames_per_line;
							if (number >= max_sprite_index)
								number = 0;
							update_bulletmenu_ents(menu->menu_items[menu->select], number);
							update_game_data("bulletdata", menu->select, number);
			}break;
		case SDLK_s:{
						hide_bulletmenu(menu);
						return_game_controller()->bullet_edit = false;
						menu->active = 0;
		}
		}break;
		}
	}
	//
}

void get_color_inputs(color_menu* menu, const Uint8* keys){
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
							  menu_set_position(menu->cursor, vector2d(menu->cursor->position.x, menu->menu_items[menu->select]->position.y-4));
			}
				break;
			case SDLK_DOWN:{
							   menu->select++;
							   if (menu->select>2)
								   menu->select = 2;
							   menu_set_position(menu->cursor, vector2d(menu->cursor->position.x, menu->menu_items[menu->select]->position.y-4));

			}break;
			case SDLK_a:{
							char* str = malloc(10);							
							int number = menu->data[menu->select];

							number++;
							if (number > 255)
								number = 0;
							menu->data[menu->select] = number;

							sprintf(str, "%d", number);

							update_game_data("colordata", menu->select, number);

							update_text(menu->menu_items[menu->select],
								-1,
								menu->menu_items[menu->select]->text_data.size,
								str,
								menu->menu_items[menu->select]->text_data.color,
								menu->menu_items[menu->select]->position,
								false);
			}break;
			case SDLK_s:{
							set_game_state(G_Menu, 0);
			}break;
			}break;
		}
	}
	//
}

void update_menu_ent(Entity *self){
	if (self->state == ES_Dead){
		self->frame = 1;
		return;
	}
	if (self->type != ES_Icon&&self->type != ES_Pause_Items){
		self->frame += .1;
		if (self->frame > self->sprite->frames_per_line)
			self->frame = 0;
	}
	if (self->type == ES_Pause_Items){
		self->position.x = get_game_camera()->position.x + self->start_position.x;
		self->position.y = get_game_camera()->position.y + self->start_position.y;
	}
}

void menu_set_position(Entity *self, Vector2D position){
	self->position = position;
	slog("called %f %f",position.x,position.y);
}

void update_select_ents(Entity *self,EntityState state){
	self->state = state;
}

void update_bulletmenu_ents(Entity *self,int index){
	self->frame = index;
}

void hide_bulletmenu(bullet_menu* menu){
	menu->cursor->show = false;
	menu->background->show = false;
	for (int i = 0; i < 5; i++)
		menu->menu_items[i]->show = false;
}

void show_bulletmenu(bullet_menu* menu){
	menu->cursor->show = true;
	menu->background->show = true;
	for (int i = 0; i < 5; i++)
		menu->menu_items[i]->show = true;
}