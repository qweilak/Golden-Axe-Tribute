#ifndef __MODULESCENELVL1_H__
#define __MODULESCENELVL1_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "Point.h"

struct Explosion
{
	Animation animation;
	fPoint point;
};
struct Collider;

struct Potion
{
	Collider* collider;
	fPoint position;
	~Potion();
};
//Objects to draw

struct ObjectsToDraw {

	SDL_Texture* texture;
	float x;
	float y;
	float positionY;
	SDL_Rect* rect;
	float speed = 1.0f;
	bool flip = false;
	float scaleXY = 1;
	bool operator < (const ObjectsToDraw& data) const { return (positionY < data.positionY); }
};

class ModuleScenelvl1 : public Module
{
public:
	ModuleScenelvl1(bool start_enabled = true);
	~ModuleScenelvl1();

	bool Start();
	update_status Update();
	bool CleanUp();
	virtual void OnCollision(Collider* a, Collider* b);
	void AddPotion(float x, float y);
public:
	std::vector<ObjectsToDraw> vectorPcNpc;
	Collider* collider = nullptr;
	int screenXZero, worldXmax;
private:
	bool music_end = true;
	std::vector<Potion*> potion;
	SDL_Rect potionRect;
	Explosion explosions[7];
	SDL_Texture* background;
	SDL_Texture* hud;
	SDL_Rect hud_lifebars;
	SDL_Rect hud_face;
	SDL_Rect hud_magic;
};

#endif //__MODULESCENELVL1_H__