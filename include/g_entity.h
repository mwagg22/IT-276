#include "gfc_vector.h"
#include "gf2d_sprite.h"
#include <stdbool.h>

#ifndef Entity_Struct
#define Entity_Struct
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
	ES_Dash = 8,
	ES_Intro = 9,
	ES_WarpIn = 10,
	ES_WarpOut = 11
}EntityState;
#endif

#ifndef Movement_Type
#define Movement_Type
typedef enum{
	M_None = 0,
	M_Switch = 1
}MovementType;
#endif
#ifndef Entity_Type
#define Entity_Type
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
	ES_Warp = 9,
	ES_Pickup = 10,
	ES_Hazard = 11,
	ES_Menu = 12,
	ES_Transition = 13,
	ES_Icon = 14,
	ES_Ready = 15,
	ES_Healthbar = 16,
	ES_Energybar = 17,
	ES_Bossbar = 18,
	ES_Boss = 19,
	ES_Pause_Items = 20,
	ES_Cursor = 21,
	ES_BossDoor = 22,
	ES_BulletEdit_Item=23,
	ES_TextBox=24
}EntityType;
#endif

#ifndef item_s
#define item_s
typedef enum
{
	I_Health = 0,
	I_Bolt = 1,
	I_Energy = 2,
	I_Chips = 3

}Item_type;
#endif

#ifndef actioninput_s
#define actioninput_s
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
#endif

#ifndef box_s
#define box_s
typedef struct BoundBox
{
	int x, y;
	int w, h;
	int offsetx, offsety;
}BoundBox;
#endif

#ifndef sprite_index_h
#define sprite_index_h
typedef struct sprite_index
{
	Sprite *idle;
	Sprite *run;
	Sprite *hit;
	Sprite *dying;
	Sprite *jump;
	Sprite *wallSlide;
	Sprite *wallAttack;
	Sprite *dash;
	Sprite *idleAttack;
	Sprite *jumpAttack;
	Sprite *dashAttack;
	Sprite *attack1;
	Sprite *attack2;
	Sprite *attack3;
	Sprite *runAttack;
	Sprite *special1;
	Sprite *special2;
	Sprite *charge;
	Sprite *warpIn;
	Sprite *warpOut;
	Sprite *intro;
}sprite_index;
#endif

#ifndef dir_h
#define dir_h
typedef enum dir_s{
	Right,
	Left,
	Up,
	Down,
}Dir;
#endif

#ifndef proj_h
#define proj_h
typedef struct Projectiles_s
{
	//struct Entity_S *Hitarray;
	EntityType parentType;
	int effect;
	int destroyeffect;
	bool destroyOnCollision;
	bool destroyOnSurface;
	bool gravity;
	bool stick;
	int aliveFrame;
	int heightTime;//how long to stay in air
	Vector2D direction;
}Projectiles;
#endif

#ifndef effect_h
#define effect_h
typedef struct Effects_s
{
	//struct Entity_S *Hitarray;
	bool centered;
	Vector2D offset;
	int type;
}Effects;
#endif

#ifndef weapons_h
#define weapons_h
typedef struct Weapons_S
{
	//-1 for not active, 0 for have, >0 ammo
	int* weaponsList;
	int currentWeapon;
}Weapons;
#endif

#ifndef text_h
#define text_h
typedef struct text_info{
	int aliveFrame;
	int size;
	char* string;
	char* font;
	bool follow;
	Vector4D color;
}text_info;
#endif

#ifndef Entity_h
#define Entity_h
typedef struct Entity_S
{
	Uint8           _inuse;         /**<flag to keep track if this isntance is in use and should not be reassigned*/
	int				Ent_ID;			//idx in list of ents
	Sprite          *sprite;          /**<the sprite for this entity*/
	sprite_index	sprite_list;			//list of models
	Vector4D		color;		//sprite color
	float			frame;
	Vector2D        position;       /**<position of the entity in 3d space*/
	Vector2D		start_position; //start point
	Vector2D        velocity;       /**<velocity of the entity in 3d space*/
	Vector2D        acceleration;   /**<acceleration of the entity in 3d space*/
	Vector2D        scale;          /**<*please default to 1,1,1*/
	Vector2D		flip;			//keep track of flipped value
	EntityState     state;          /**<current state of the entity*/
	BoundBox		hitbox;			//hitbox box
	Dir				dir;			//controls which way they face to flip sprite
	Actioninput		action;			//keep track of input
	Actioninput		prev_action;	//prev action
	EntityType		type;			//check entity type
	Item_type		itemType;       //for items
	Projectiles		proj_data;		//for projectiles
	Effects			effect_data;    //for effets;
	Weapons			weapons_list;   //for player
	text_info		text_data; //text
	MovementType	movement_type;
	struct health_s*		health_bar;
	void(*think)(struct Entity_S* self);   /**<function called on entity think*/
	void(*update_sprite)(struct Entity_S* self);   /**<function called on entity update*/
	void(*setPosition)(struct Entity_S* self, Vector2D position);   /**set position*/
	void(*update_ent)(struct Entity *self);
	void(*move)(struct Entity_S* self);
	void(*attack)(struct Entity_S* self);
	void(*victory)(struct Entity_S* self);
	void(*special)(struct Entity_S* self, int n);
	void(*getPowerUp)(struct Entity_S* self, int n);
	void(*transition_function)(struct Entity_S* self, Dir dir);
	void(*onDeath)(struct Entity_S* self);
	void(*get_inputs)(struct Entity *self, const Uint8 * keys, float delta);
	void(*damage)(struct Entity_S *self, int damage, Vector2D kick);
	void(*reset)(struct Entity_S* self);
	int           health;
	int           healthmax;
	int           defense;
	int			  bolts;
	int           attackdmg;
	int           mp;
	float         movementspeed;
	float		  dashspeed;
	int 		  controlling;
	int			  specialnum;
	int           attacknum;
	int			  maxjump;
	int			  maxdash;
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
	bool			l_wall_collision;//for wall jump, check left and right colliision
	bool			r_wall_collision;
	bool			bossdoor_collision;
	bool			ladder_collision;
	bool upper_wall_collision;
	bool right_trigger;
	bool left_trigger;
	bool up_trigger;
	bool down_trigger;
	bool on_ladder;
	bool is_wall_sliding;
	bool jumped;
	bool wall_jumped;
	bool falling;
	bool respawn;
	bool transition;
	bool active;
	bool did_intro;
	bool health_created;
	Dir transition_dir;
	int in_air;
	int				heldFrame; //count number of frames held
	int				dashFrame;//dash frames
	int				jumpFrame;//jump frames
	int				invincibleFrame;//invincibility frames
	int actionFrame;//how long to hold an action
	int damageFrame;//how long to flash
	int				comboNum;
	int clip;//if solid or not
	int energy;
	void			*data;                     /**<additional entity specific data*/
	struct Entity_S *target;//for enemies
	struct Entity_S *parent;//for projectiles
	Vector2D topbounds;//for warps
	Vector2D bottombounds;//for warps
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
void set_to_zero_ent(int i);
void hide(Entity* toHide);
void show(Entity* toShow);
Entity *gf2d_return_list();
Entity *return_game_list();
void hide_all(Entity *list);
void show_all(Entity *list);
void gf2d_clear_entity_manager();
void update_entity(Entity *self);

void gf2d_entity_free(Entity *self);
void set_hitbox(Entity *self, int x, int y, int w, int h, int offsetx, int offsety);
void update_hitbox_position(Entity *self);
void set_position(Entity *self, Vector2D position);
void displacement(Entity *self, Vector2D disp);
void init_ent(Entity *self, int cont);
void update_sprite(Entity *self);
void update_ent_sprite(Entity *self);
void update_ent(Entity *self);
Entity *get_nearest_target(Entity *self, Entity *other);
void draw_entities(struct Camera_S* cam);
void draw_ui(struct Camera_S* cam);
void draw_text();
void update_all_entities(int type);
Entity *get_player_entity();
Entity *get_boss_entity();
void respawn(Entity *self);
void flip(Entity *self, Vector2D position);
void entity_in_bounds(Entity* self, struct Camera_S *cam);
void entity_tile_collision(int** tiles);
void init_ready_ent(Entity* self);
void update_ready_ent(Entity *self);
void init_color_ent(Entity* self);
void update_color_ent(Entity* self);
void init_bossdoor_ent(Entity* self, int type);
void update_bossdoor_ent(Entity *self);
void reset_bossdoor(Entity* self);
Entity *get_bar_entity(EntityType type);

//transition
void init_transition_ent(Entity* self, Vector2D topleft, Vector2D bottomright, Dir dir);
#endif