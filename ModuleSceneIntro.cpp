#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"
#include "ModuleScenelvl1.h"
#include "ModuleSceneIntro.h"

ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module(start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	graphics = App->textures->Load("_png_intro.png");
	App->audio->PlayMusic("_music_Golden_Axe_.ogg", 1.0f);
	fx = App->audio->LoadFx("_fx-selection.wav");
	App->renderer->camera.x = App->renderer->camera.y = 0;

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	App->textures->Unload(graphics);

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
	{
		App->audio->PlayFx(fx);
		App->fade->FadeToBlack(App->scene_lvl1, this, 1.0f);
	}

	// Draw everything --------------------------------------	
	App->renderer->Blit(graphics, 0, 0, NULL);

	return UPDATE_CONTINUE;
}