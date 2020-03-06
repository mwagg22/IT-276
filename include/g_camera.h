#ifndef camera_h
#define camera_h
#include "g_entity.h"

typedef struct Camera_S{
	Vector2D position;
	int viewWidth;
	int viewHeight;
	Vector2D topLeftBounds;
	Vector2D bottomRightBounds;
	Entity *target;
	bool follow;
}Camera;


Camera* init_camera(Entity* target, int viewWidth, int viewHeight, Vector2D topLeftBounds, Vector2D bottomRightBounds);
void set_bounds(Camera* cam, Vector2D topLeftBounds, Vector2D bottomRightBounds);
void update_camera(Camera* cam);
void move_to_new_position(Camera *cam, Vector2D position);
Vector2D distance_from_camera(Camera *cam, Entity* target);
bool in_camera_bounds(Camera *cam, Entity *self);
bool vector_in_camera_bounds(Camera *cam, Vector2D vector);
void set_target(Camera* cam, Entity *target, int follow);
#endif