#ifndef __MODULEPLAYER_H__
#define __MODULEPLAYER_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "Point.h"
#include "Timer.h"
#include "ModuleSword.h"

class ModuleLongmoan;
struct SDL_Texture;
struct Collider;

class ModulePlayer : public Module
{
public:
	ModulePlayer(bool start_enabled = true);
	~ModulePlayer();

	bool Start();
	bool CleanUp();
	update_status Update();

	void OnCollisionBegin(Collider* a, Collider* b);
	fPoint GetOffset() const { return offset; };
	
	PlayerState GetState() const { return state; };
	void SetState(PlayerState s) { state = s; };
	
	bool GetFlip() const { return flip; };
	uint32 GetTimeOfAnimation() { return timeAnim.Read(); };

private:

	void OnAttack(Animation* anim, uint fx);
	
public:

	SDL_Texture* graphics = nullptr;
	
	fPoint position;
	uint potions;
	int lifePoints;

	Animation GetAnimation(int i) { return animationsArray[i]; };
	Animation* GetCurrentAnimation() { return current_animation; };

	ModuleLongmoan* left;
	ModuleLongmoan* right;

private:

	Uint8 speed;
	uint fx[4];
	bool fx_played = false;
	PlayerState state;
	fPoint offset;
	bool flip;
	Collider* collider = nullptr;
	Sword sword;
	uint timeMax = 0;
	Timer timeAnim, timeWindow, triggerRun;
	int canRun = 0;
	Animation* current_animation = nullptr;
	Animation animationsArray[PLAYER_NUM_OF_ANIMATIONS];
};

#endif // __MODULEPLAYER_H__