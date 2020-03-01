#include "g_camera.h"

Camera* init_camera(Entity* target, int viewWidth, int viewHeight, Vector2D topLeftBounds, Vector2D bottomRightBounds){
	Camera* cam = malloc(sizeof(Camera));
	cam->viewWidth = viewWidth;
	cam->viewHeight= viewHeight;
	cam->topLeftBounds = topLeftBounds;
	cam->bottomRightBounds = bottomRightBounds;
	cam->target = target;
	cam->follow = 1;
	return cam;
}

void set_bounds(Camera* cam, Vector2D topLeftBounds, Vector2D bottomRightBounds){
	cam->topLeftBounds = topLeftBounds;
	cam->bottomRightBounds = bottomRightBounds;
}
void update_camera(Camera* cam){
	cam->position.x = cam->target->position.x - 180;
	cam->position.y = cam->target->position.y - 80;
	if (cam->position.x < cam->topLeftBounds.x){
		cam->position.x = cam->topLeftBounds.x;
	}
	if (cam->position.y < cam->topLeftBounds.y){
		cam->position.y = cam->topLeftBounds.y;
	}
	if (cam->position.x > cam->bottomRightBounds.x){
		cam->position.x = cam->bottomRightBounds.x;
	}
	if (cam->position.y > cam->bottomRightBounds.y){
		cam->position.y = cam->bottomRightBounds.y;
	}
	
}
void move_to_new_position(Camera *cam, Vector2D newPosition){
	;
}
Vector2D distance_from_camera(Camera *cam, Entity* target){
	Vector2D c;
	vector2d_sub(c, cam->position, target->position);
	return c;
}

bool in_camera_bounds(Camera *cam, Entity *self){
	return(self->position.x + self->hitbox.offsetx + self->hitbox.w > cam->position.x&&
		self->position.x + self->hitbox.offsetx < cam->position.x + cam->viewWidth&&
		self->position.y + self->hitbox.offsety + self->hitbox.h > cam->position.y&&
		self->position.y + self->hitbox.offsety < cam->position.y + cam->viewHeight);
}

bool vector_in_camera_bounds(Camera *cam, Vector2D vector){
	return(vector.x > cam->position.x&&
		vector.x < cam->position.x + cam->viewWidth&&
		vector.y > cam->position.y&&
		vector.y < cam->position.y + cam->viewHeight);
}