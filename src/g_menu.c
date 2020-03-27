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
	menu->menu_items[1] = *save;
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

	Entity *background = gf2d_entity_new();
	init_item(background, 11);

	Entity *cursor = gf2d_entity_new();
	init_item(cursor, 10);

	Entity *start = gf2d_entity_new();
	init_item(start, 12);

	Entity *save = gf2d_entity_new();
	init_item(save, 13);

	menu->menu_items[0] = start;
	menu->menu_items[1] = save;
	menu->cursor = cursor;
	menu->select = 0;
	menu->active = 0;

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
				   self->sprite = gf2d_sprite_load_all("../images/test/menu/pause_continue.png", 90, 14, 1);
				   self->start_position = vector2d(42, 89);
				   self->position = self->start_position;
				   self->type = ES_Pause_Items;
	}break;
	case(13) : {
				   self->sprite = gf2d_sprite_load_all("../images/test/menu/pause_quit.png", 44, 18, 1);
				   self->start_position = vector2d(42, 128);
				   self->position = self->start_position;
				   self->type = ES_Pause_Items;
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
							   if (menu->select>1)
								   menu->select = 1;
							   menu_set_position(menu->cursor, vector2d(menu->cursor->position.x, menu->menu_items[menu->select].position.y));
						
			}break;
			case SDLK_a:{
							if (menu->select == 0){
								play_soundeffect("../sounds/warp.wav", 0);
								set_game_state(G_BossSelect, 0);
							}
							else{
								//load save file
								play_soundeffect("../sounds/warp.wav", 0);
								load_save();
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
							   if (menu->select>1)
								   menu->select = 1;
							   menu_set_position(menu->cursor, vector2d(menu->cursor->position.x, menu->menu_items[menu->select]->position.y));

			}break;
			case SDLK_a:{
							if (menu->select == 0){
								return_game_controller()->pause = false;
								set_update_type(0);
								menu->active = 0;
							}								
							else{
								//load save file
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

void update_menu_ent(Entity *self){
	if (self->state == ES_Dead){
		self->frame = 1;
		return;
	}
	if (self->type != ES_Icon){
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