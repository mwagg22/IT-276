#include "gfc_vector.h"
#include "gf2d_sprite.h"
#include <stdbool.h>

typedef enum
{
	ES_Idle = 0,
	ES_Running = 1,
	ES_Attacking = 2,
	ES_Special = 3,
	ES_Hit = 4,
	ES_Dying = 5,
	ES_Dead = 6,
	ES_Jump = 7,
	ES_Dash = 8
}EntityState;

typedef enum
{
	ES_Player = 0,
	ES_Item = 1,
	ES_Weapon = 2,
	ES_Projectile = 3,
	ES_Hitbox = 4,
	ES_Effect = 5,
	ES_NPC = 6,
	ES_Enemy = 7,
	ES_Stage = 8,
	ES_Warp = 9
}EntityType;

typedef enum
{
	attack = 0,
	movement = 1,
	special = 2,
	hpskill = 3,
	block = 4,
	menu = 5,
	none = 6,
	jump = 7,
	dash = 8
}Actioninput;

typedef struct BoundBox
{
	int x, y;
	int w, h;
}BoundBox;

typedef struct sprite_index
{
	Sprite *idle;
	Sprite *run;
	Sprite *hit;
	Sprite *dying;
	Sprite *jump;
	Sprite *dash;
	Sprite *attack1;
	Sprite *attack2;
	Sprite *attack3;
	Sprite *attack4;
	Sprite *special1;
	Sprite *special2;
	Sprite *charge;
}sprite_index;

typedef enum{
	Right,
	Left,
	Up,
	Down,
}dir;

typedef struct Entity_S
{
	Uint8           _inuse;         /**<flag to keep track if this isntance is in use and should not be reassigned*/
	int				Ent_ID;			//idx in list of ents
	Sprite          *sprite;          /**<the sprite for this entity*/
	sprite_index	sprite_list;			//list of models
	Vector4D		color;		//sprite color
	float			frame;
	Vector2D        position;       /**<position of the entity in 3d space*/
	Vector2D        velocity;       /**<velocity of the entity in 3d space*/
	Vector2D        acceleration;   /**<acceleration of the entity in 3d space*/
	Vector2D        scale;          /**<*please default to 1,1,1*/
	Vector2D		flip;			//keep track of flipped value
	EntityState     state;          /**<current state of the entity*/
	BoundBox		hitbox;			//hitbox box
	dir				dir;			//controls which way they face to flip sprite
	Actioninput		action;			//keep track of input
	Actioninput		prev_action;	//prev action
	EntityType		type;			//check entity type
	struct health_s*		health_bar;
	void(*think)(struct Entity_S* self);   /**<function called on entity think*/
	void(*update_sprite)(struct Entity_S* self);   /**<function called on entity update*/
	void(*touch)(struct Entity_S* self, struct Entity_S* other);   /**<function called on entity think*/
	void(*update_ent)(struct Entity *self);
	void(*move)(struct Entity_S* self);
	void(*attack)(struct Entity_S* self);
	void(*block)(struct Entity_S* self);
	void(*special)(struct Entity_S* self, int n);
	void(*useItem)(struct Entity_S* self);
	void(*get_inputs)(struct Entity *self, const Uint8 * keys, float delta);
	int           health;
	int           healthmax;
	int           defense;
	int           attackdmg;
	int           mp;
	float         movementspeed;
	float		  dashspeed;
	int 		  controlling;
	int			  specialnum;
	int           attacknum;
	int           cast;//for magic casting
	bool          show;
	bool			can_attack;
	bool			can_special;
	bool			is_hit;
	bool			in_action;
	bool			is_collision;
	bool			is_held;		//check if button held
	bool			is_grounded;
	bool			is_dashing; //check if dashing...lotta redundent things here
	bool            attack_trigger; //rework attack remove attack from entitystate
	bool			in_attack;//in attaciking state
	int				heldFrame; //count number of frames held
	int				dashFrame;//dash frames
	int				jumpFrame;//jump frames
	int				comboNum;
	void			*data;                     /**<additional entity specific data*/
	struct Entity_S *target;//for enemies
	//struct Entity_S *parent;//for projectiles
	Vector2D location;//for warps
}Entity;



/**
 * @brief initializes the entity subsystem
 * @param entity_max maximum number of simultaneous entities you wish to support
 */
void gf2d_entity_manager_init(Uint32 entity_max);

/**
 * @brief get an empty entity from the system
 * @return NULL on out of space or a pointer to an entity otherwise
 */
Entity *gf2d_entity_new();
void hide(Entity* toHide);
void show(Entity* toShow);
Entity *gf2d_return_list();
Entity *return_game_list();
void hide_all(Entity *list);
void show_all(Entity *list);


void gf2d_entity_free(Entity *self);
void gf2d_set_boundbox(Entity *self, Vector2D min, Vector2D max);
void set_position(Entity *self, Vector2D position);
void displacement(Entity *self, Vector2D disp);
void init_ent(Entity *self, int cont);
void update_sprite(Entity *self);
void update_ent_sprite(Entity *self);
void update_ent(Entity *self);
Entity *get_nearest_target(Entity *self, Entity *other);
void set_hitbox(Entity *self, int width, int height);
void free_ent_manager(int i);
void draw_entities();