#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleFadeToBlack.h"
#include "ModuleCollision.h"
#include "ModuleSceneIntro.h"
#include "ModuleScenelvl1.h"
#include "ModulePlayer.h"
#include "ModuleEnemies.h"
#include "ModuleSword.h"
#include <vld.h>
using namespace std;

Application::Application()
{
	// Order matters: they will init/start/pre/update/post in this order
	modules.push_back(input = new ModuleInput());
	modules.push_back(window = new ModuleWindow());

	modules.push_back(textures = new ModuleTextures());
	modules.push_back(audio = new ModuleAudio());
	modules.push_back(swords = new ModuleSword());
	modules.push_back(collision = new ModuleCollision());
	modules.push_back(fade = new ModuleFadeToBlack());

	// Game Modules
	modules.push_back(scene_lvl1 = new ModuleScenelvl1(false));
	modules.push_back(scene_intro = new ModuleSceneIntro(false));
	modules.push_back(player = new ModulePlayer(false)); // player needs to be after scene
	modules.push_back(enemies = new ModuleEnemies(false));
	
	modules.push_back(renderer = new ModuleRender());
	// set first scene for the game
	first_scene = scene_intro;
}

Application::~Application()
{
	modules.clear();

	delete renderer;
	delete enemies;
	delete player;
	delete scene_intro;
	delete scene_lvl1;
	delete fade;
	delete collision;
	delete swords;
	delete audio;
	delete textures;
	delete input;
	delete window;
	
}

bool Application::Init()
{
	bool ret = true;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init(); // we init everything, even if not anabled

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
	{
		if((*it)->IsEnabled() == true)
			ret = (*it)->Start();
	}

	// Start the first scene --
	if(first_scene != nullptr)
		fade->FadeToBlack(first_scene, nullptr, 3.0f);

	return ret;
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->PreUpdate();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->Update();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->PostUpdate();

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for(list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->CleanUp();

	return ret;
}

