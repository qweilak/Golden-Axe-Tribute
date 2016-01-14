#ifndef __MODULELONGMOAN__H
#define __MODULELONGMOAN__H

#include "Globals.h"
#include "Module.h"
#include "Animation.h"
#include "Point.h"
#include "Timer.h"
#include "ModuleSword.h"


enum EnemyType { NORMAL, BOSS };
struct SDL_Texture;
struct Collider;


class ModuleLongmoan : public Module
{
public:
	ModuleLongmoan(bool start_enabled = true);
	~ModuleLongmoan();

	void InitLongmoan();
	void SetBoss();
	update_status Update();
	void BestAction();
	void OnCollisionBegin(Collider* c1, Collider* c2);
	fPoint GetOffset() const { return offset; };
	bool GetFlip() const { return flip; };
	EnemyType GetType() const { return type; };

	bool CompareAnimations (int i) { return (current_animation == &animArraY[i]); };
	void SetAnimation(int i) { current_animation = &animArraY[i]; };
	Animation* GetCurrentAnimation() { return current_animation; };
	
public:

	EnemyState state;
	int livePoints;
	fPoint position;	
	float scaleXY = 1.0f;

private:

	bool fx_played = false;
	fPoint offset;
	Collider* collider = nullptr;
	bool flip;
	Timer time;
	uint randomTime;
	fPoint target;
	Sword sword;
	EnemyType type;
	uint fx[2];
	float speed;
	Animation* current_animation = nullptr;
	Animation animArraY[ENEMY_NUM_OF_ANIMATIONS];
};

#endif //__MODULELONGMOAN__H