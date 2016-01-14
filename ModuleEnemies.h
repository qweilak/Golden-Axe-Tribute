#pragma once

#include "Globals.h"
#include "Module.h"

class ModuleLongmoan;
class Animation;

struct SDL_Texture;

class ModuleEnemies : public Module
{
public:
	ModuleEnemies(bool start_enabled = true);
	~ModuleEnemies();

	bool Start();
	bool CleanUp();

	update_status Update();

	const ModuleLongmoan* GetEnemy(int i) const { return LongMoan[i]; };
	SDL_Texture*  GetGraphics() const { return graphics; };

	Animation* GetCurrentAnimation(int i);

public:

	bool stopEnemies = false;
	bool startEnemies = true;
	bool checkGameFinished();

private:

	ModuleLongmoan* LongMoan[NUM_OF_ENEMIES];
	SDL_Texture* graphics = nullptr;
};

