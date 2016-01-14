#ifndef __MODULECOLLISION_H__
#define __MODULECOLLISION_H__

#include "Module.h"
#include <list>

enum COLLIDER_TYPE
{
	COLLIDER_NONE = -1,
	COLLIDER_PLAYER,
	COLLIDER_ENEMY,
	COLLIDER_PLAYER_SHOT,
	COLLIDER_ENEMY_SHOT,
	COLLIDER_CONTINUE,
	COLLIDER_COLLECTIBLE,
	COLLIDER_MAX
};

struct Collider
{
	SDL_Rect rect;
	bool to_delete;
	COLLIDER_TYPE type;
	Module* callback;

	Collider(SDL_Rect rectangle, COLLIDER_TYPE type, Module* callback = NULL) :
		rect(rectangle),
		type(type),
		callback(callback),
		to_delete(false)
	{}

	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	bool CheckCollision(SDL_Rect r) const;
};

struct DoubleColliders{ const Collider* c1; const Collider* c2; };

class ModuleCollision : public Module
{
public:

	ModuleCollision();
	~ModuleCollision();

	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	Collider* AddCollider(SDL_Rect rect, COLLIDER_TYPE type, Module* callback = NULL);
	bool CheckStatus(const Collider* c1, const Collider* c2);

private:

	std::list<Collider*>	colliders;
	std::list<DoubleColliders>	collisions;
	DoubleColliders statusCollision;
	bool matrix[COLLIDER_MAX][COLLIDER_MAX];
	bool debug;
};

#endif // __MODULECOLLISION_H__