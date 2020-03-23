#include "g_camera.h"
#include "g_game.h"

Camera* init_camera(Entity* target, int viewWidth, int viewHeight, Vector2D topLeftBounds, Vector2D bottomRightBounds){
	Camera* cam = malloc(sizeof(Camera));
	cam->viewWidth = viewWidth;
	cam->viewHeight= viewHeight;
	cam->topLeftBounds = topLeftBounds;
	cam->bottomRightBounds = bottomRightBounds;
	cam->transition = false;
	cam->can_move = true;
	if (target != NULL){
		cam->target = target;
		cam->follow = 1;
	}
	else{
		cam->follow = 0;
		cam->position.x = 0;
		cam->position.y = 0;
	}
	return cam;
}
void set_target(Camera* cam, Entity *target,int follow){
	cam->follow = follow;
	cam->target = target;
}
void set_bounds(Camera* cam, Vector2D topLeftBounds, Vector2D bottomRightBounds){
	cam->topLeftBounds = topLeftBounds;
	cam->bottomRightBounds = bottomRightBounds;
}
void update_camera(Camera* cam){
	if (cam->transition == true){
		if (in_new_position_bounds(cam)){
			switch (cam->dir){
			case(Right) : {
							  slog("transitioning");
							  cam->position.x += 2;
							  
			}break;
			case(Left) : {
							  slog("transitioning");
							  cam->position.x -= 2;

			}break;
			case(Up) : {
							 slog("transitioning");
							 cam->position.y -= 2;

			}break;
			case(Down) : {
							 slog("transitioning");
							 cam->position.y += 2;

			}break;
			}
		}
		else{
			//turn everything back
			slog("done transitioning");
			set_input_control(1);
			return_game_controller()->controllerEntity->transition = false;
			set_update_type(0);
			cam->transition = false;
			
		}

	}
	else{
		if (cam->follow == 1){
			if (cam->can_move==true){
				cam->position.x = cam->target->position.x - cam->viewWidth / 2.5;
				cam->position.y = cam->target->position.y - cam->viewHeight / 2;
			}
		}
		else{
			cam->position.x = cam->position.x;
			cam->position.y = cam->position.y;
		}
		if (cam->position.x < cam->topLeftBounds.x){
			cam->position.x = cam->topLeftBounds.x;
		}
		if (cam->position.y < cam->topLeftBounds.y){
			cam->position.y = cam->topLeftBounds.y;
		}
		if (cam->position.x +cam->viewWidth >= cam->bottomRightBounds.x){
			//cam->can_move = false;
			cam->position.x = cam->bottomRightBounds.x - cam->viewWidth;
		}
		if (cam->position.y +cam->viewHeight >= cam->bottomRightBounds.y){
			//cam->can_move = false;
			cam->position.y = cam->bottomRightBounds.y -cam->viewHeight;
		}
	}
	
}
void move_to_new_position(Camera *cam, Vector2D topleft,Vector2D bottomRight,Dir dir){
	cam -> transition = true;
	cam->topLeftBounds = topleft;
	cam->bottomRightBounds = bottomRight;
	cam->dir = dir;
}
Vector2D distance_from_camera(Camera *cam, Entity* target){
	Vector2D c;
	vector2d_sub(c, cam->position, target->position);
	return c;
}

bool in_camera_bounds(Camera *cam, Entity *self){
	return(self->position.x + self->hitbox.offsetx + self->hitbox.w > cam->position.x&&
		self->position.x + self->hitbox.offsetx <= cam->position.x + cam->viewWidth&&
		self->position.y + self->hitbox.offsety + self->hitbox.h > cam->position.y&&
		self->position.y + self->hitbox.offsety <= cam->position.y + cam->viewHeight);
}

bool vector_in_camera_bounds(Camera *cam, Vector2D vector){
	return(vector.x > cam->position.x&&
		vector.x < cam->position.x + cam->viewWidth&&
		vector.y > cam->position.y&&
		vector.y < cam->position.y + cam->viewHeight);
}

bool in_new_position_bounds(Camera *cam){
	//should check by direction
	if (cam->dir == Right){
		return(cam->topLeftBounds.x >= cam->position.x&&
			cam->bottomRightBounds.x >= cam->position.x + cam->viewWidth);
	}
	else if (cam->dir == Left){
		return(cam->topLeftBounds.x <= cam->position.x&&
			cam->bottomRightBounds.x <= cam->position.x + cam->viewWidth);
	}
	else if (cam->dir == Up){
		return(cam->topLeftBounds.y <= cam->position.y&&
			cam->bottomRightBounds.y <= cam->position.y + cam->viewHeight);
	}
	else if (cam->dir == Down){
		return(cam->topLeftBounds.y >= cam->position.y&&
			cam->bottomRightBounds.y >= cam->position.y + cam->viewHeight);
	}
}