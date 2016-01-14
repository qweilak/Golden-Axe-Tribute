#ifndef __MODULESWORD_H__
#define __MODULESWORD_H__

#include "Globals.h"
#include "Module.h"
#include "Point.h"
#include "Timer.h"
#include <list>

enum COLLIDER_TYPE;

struct Sword
{

	fPoint position = { 0, 0 };
	Timer timer;
	uint32 delay;
	uint32 born = 0;
	uint32 life = 0;
	Collider* collider = nullptr;

	~Sword();
	bool Update();
};

class ModuleSword : public Module
{
public:
	ModuleSword();
	~ModuleSword();

	update_status PostUpdate();

	void OnCollisionBegin(Collider*, Collider*);
	void AddSword(const Sword& sword, float x, float y, COLLIDER_TYPE, Uint32 delay = 0);
	
private:

	std::list<Sword*> active;

};

#endif // __MODULESWORD_H__