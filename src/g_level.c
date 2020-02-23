#include "g_level.h"
#include <stdio.h>
#include <string.h>


Level* load_level(SJson* filename){
	Level *level;
	return level;
}
char** return_tiles(Level* level){
	return level->tiles;
}
int return_level_width(Level* level){
	return level->width;
}
int return_level_height(Level* level){
	return level->height;
}
