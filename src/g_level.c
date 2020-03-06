#include "g_level.h"
#include <stdio.h>
#include <string.h>
#include "simple_logger.h"

Level* load_level(const char* filename){
	Level *level = (Level *)malloc(sizeof(Level));
	SJson *value,*file,*arr;
	const char* tilesheet;
	int numOfTiles;
	level->color = vector4d(255, 255, 255, 255);

	file = sj_load(filename);
	if (file == NULL){
		slog("file not found");
		return level;
	}
	
	value = sj_object_get_value(file, "height");
	sj_get_integer_value(value, &level->height);
	slog("here %i",level->height);
	value = sj_object_get_value(file, "width");
	sj_get_integer_value(value, &level->width);

	value = sj_object_get_value(file, "layers");
	level->tiles = sj_array_2d(sj_array_get_nth(value, 0), level->height, level->width);
	load_level_entities(sj_object_get_value(file, "objects"));

	value = sj_object_get_value(file, "tileheight");
	sj_get_integer_value(value, &level->tileheight);

	value = sj_object_get_value(file, "tilewidth");
	sj_get_integer_value(value, &level->tilewidth);

	value = sj_object_get_value(file, "tilesheet");
	tilesheet = sj_get_string_value(value);

	value = sj_object_get_value(file, "tileimages");
	sj_get_integer_value(value, &numOfTiles);

	level->tilesheet = gf2d_sprite_load_all(tilesheet, level->tilewidth, level->tileheight, numOfTiles);

	slog("Level loaded: %s height,width:%i,%i tileheight,tilewidth:%i,%i ", level->tilesheet->filepath, level->height, level->width, level->tileheight, level->tilewidth);

	return level;
}
void load_level_entities(SJson* list){
	SJson *value, *object,*objectContent;
	int ctr = 0;
	for (int j = 0; j < sj_array_get_count(list); j++){
		object = sj_array_get_nth(list, ctr);
		objectContent = sj_object_get_value(object, "name");
		Entity* entity = gf2d_entity_new();

		if (!strcmp(sj_get_string_value(objectContent),"player")){
			slog("player object");
			init_gungirl_ent(entity, 1);
		}
		else if (!strcmp(sj_get_string_value(objectContent), "item")){
			//based on properties
			int val = 0;
			objectContent = sj_object_get_value(object, "properties");
			objectContent = sj_array_get_nth(objectContent,0);
			objectContent = sj_object_get_value(object, "value");
			sj_get_integer_value(value, &val);
			init_item_ent(entity, val);
		}
		else if (!strcmp(sj_get_string_value(objectContent), "shooterenemy")){
			init_shooterenemy_ent(entity, 0);
		}
		else if (!strcmp(sj_get_string_value(objectContent), "rollerenemy")){
			init_rollerenemy_ent(entity, 0);
		}
		else if (!strcmp(sj_get_string_value(objectContent), "lookerenemy")){
			init_lookerenemy_ent(entity, 0);
		}
		else if (!strcmp(sj_get_string_value(objectContent), "sliderHazard")){
			init_hazard_ent(entity, 0);
		}
		else if (!strcmp(sj_get_string_value(objectContent), "flameHazard")){
			init_hazard_ent(entity, 1);
		}
		else if (!strcmp(sj_get_string_value(objectContent), "rockHazard")){
			init_hazard_ent(entity, 2);
		}

		ctr++;
		int x, y;

		objectContent = sj_object_get_value(object, "x");
		sj_get_integer_value(objectContent, &x);
		objectContent = sj_object_get_value(object, "y");
		sj_get_integer_value(objectContent, &y);
		set_position(entity, vector2d(x, y));

	}
}

int** return_tiles(Level* level){
	return level->tiles;
}
int return_level_width(Level* level){
	return level->width;
}
int return_level_height(Level* level){
	return level->height;
}
void draw_tiles(Level* level,Camera* cam){
	for (int i = cam->position.y / 16; i < (cam->position.y + cam->viewHeight) / 16; i++){
		for (int j = cam->position.x / 16; j < (cam->position.x + cam->viewWidth) / 16; j++){
			if (level->tiles[i][j] == 0)
				continue;
			gf2d_sprite_draw(level->tilesheet, vector2d(j * 16, i * 16), cam->position, NULL, NULL, NULL, NULL, &level->color, level->tiles[i][j]-1);
		}
	}
}