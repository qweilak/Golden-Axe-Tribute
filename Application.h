#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include<list>
#include<vector>
#include "Globals.h"
#include "Module.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleAudio;
class ModuleFadeToBlack;
class ModulePlayer;
class ModuleCollision;
class ModuleSceneIntro;
class ModuleScenelvl1;
class ModuleEnemies;
class ModuleSword;
class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

public:
	ModuleRender* renderer;
	ModuleWindow* window;
	ModuleTextures* textures;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleFadeToBlack* fade;
	ModuleSword* swords;
	ModuleCollision* collision;

	// Game logic --
	ModulePlayer* player;
	ModuleSceneIntro* scene_intro;
	ModuleScenelvl1* scene_lvl1;
	ModuleEnemies* enemies;

private:

	std::list<Module*> modules;
	Module* first_scene = nullptr;
};

extern Application* App;

#endif // __APPLICATION_H__