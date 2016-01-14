#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include "SDL/include/SDL_rect.h"

typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned __int32 uint32;

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

enum PlayerState { 

	PLAYER_DEAD,
	PLAYER_IDLE,
	PLAYER_MOVE,
	PLAYER_JUMP,
	PLAYER_ATTACK,
	PLAYER_COMBO_1,
	PLAYER_COMBO_2,
	PLAYER_MAGIC,
	PLAYER_HURT,

};

enum EnemyState { 

	ENEMY_OUTOFSCREEN,
	ENEMY_DEAD,
	ENEMY_IDLE,
	ENEMY_MOVE,
	ENEMY_HURT, 
	ENEMY_TARGETSIDE, 
	ENEMY_ATTACK,  
	ENEMY_COMBO, 
	ENEMY_MAGIC,
	ENEMY_RUNNING,
	ENEMY_WAIT

};

enum EnemyAnimation {

	ENEMY_IDLE_ANIMATION,
	ENEMY_FORWARD_ANIMATION,
	ENEMY_UP_ANIMATION,
	ENEMY_HURT_ANIMATION,
	ENEMY_MAGIC_HURT_ANIMATION,
	ENEMY_DEAD_ANIMATION,
	ENEMY_ATTACK_ANIMATION,
	ENEMY_RUN_ANIMATION,
	ENEMY_NUM_OF_ANIMATIONS

};

enum PlayerAnimation {

	PLAYER_IDLE_ANIMATION,
	PLAYER_FORWARD_ANIMATION,
	PLAYER_UP_ANIMATION,
	PLAYER_HURT_ANIMATION,
	PLAYER_MAGIC_ATTACK_ANIMATION,
	PLAYER_MAGIC_EXPLOSIONS_ANIMATION,
	PLAYER_DEAD_ANIMATION,
	PLAYER_ATTACK_ANIMATION,
	PLAYER_COMBO1_ANIMATION,
	PLAYER_COMBO2_ANIMATION,
	PLAYER_PREJUMP_ANIMATION,
	PLAYER_JUMP_UP_ANIMATION,
	PLAYER_JUMP_DOWN_ANIMATION,
	PLAYER_JUMP_ATTACK_ANIMATION,
	PLAYER_RUN_ANIMATION,
	PLAYER_NUM_OF_ANIMATIONS
};


// Deletes a buffer
#define RELEASE( x ) \
    { \
       if( x != nullptr ) \
       { \
         delete x; \
	     x = nullptr; \
       } \
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x ) \
	{  \
       if( x != nullptr ) \
       { \
           delete[] x; \
	       x = nullptr; \
		 } \
	 }

// Configuration -----------
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 224
#define SCREEN_SIZE 3
#define FULLSCREEN false
#define VSYNC true
#define TITLE "Golden Axe Tribute"
#define SCREEN_MARGIN 32

#define NUM_OF_ENEMIES 12

#define COLLIDER_PLAYER_WIDTH 44
#define COLLIDER_PLAYER_HEIGHT 4
#define COLLIDER_ENEMY_WIDTH 48
#define COLLIDER_ENEMY_HEIGHT 4
#define COLLIDER_SWORD_WIDTH 16
#define COLLIDER_SWORD_HEIGHT 6

#define LOWER_PATH SCREEN_HEIGHT - 6
#define MIDDLE_PATH SCREEN_HEIGHT - 40
#define UPPER_PATH SCREEN_HEIGHT - 72
#define BOSS_SCALE 1.3f
#define HEIGHT_OF_PATH 80
#define YMIN (SCREEN_HEIGHT - HEIGHT_OF_PATH)
#define YMAX SCREEN_HEIGHT

#define PLAYER_SPEED_WALK 2
#define PLAYER_SPEED_RUNNING 4

#define LONGMOAN_SPEED_WALK 0.5f
#define LONGMOAN_SPEED_RUNNING 2.0f

#endif //__GLOBALS_H__