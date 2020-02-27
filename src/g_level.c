#include "g_level.h"
#include <stdio.h>
#include <string.h>
#include "simple_logger.h"

Level* load_level(SJson* json){
	Level *level = (Level *)malloc(sizeof(Level));
	SJson *value,*file,*arr;
	const char* tilesheet;
	int numOfTiles;
	level->color = vector4d(255, 255, 255, 255);

	file = sj_load(json);
	if (file == NULL){
		slog("file not found");
		return level;
	}

	value = sj_object_get_value(file, "height");
	sj_get_integer_value(value, &level->height);
	
	value = sj_object_get_value(file, "width");
	sj_get_integer_value(value, &level->width);

	value = sj_object_get_value(file, "layers");
	level->tiles = sj_array_2d(sj_array_get_nth(value, 0), level->height, level->width);

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