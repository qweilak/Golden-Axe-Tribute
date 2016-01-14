#include "Application.h"
#include "ModuleEnemies.h"
#include "ModuleLongmoan.h"
#include "ModulePlayer.h"
#include "ModuleCollision.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleScenelvl1.h"


ModuleEnemies::ModuleEnemies(bool start_enabled) : Module(start_enabled)
{
}


ModuleEnemies::~ModuleEnemies()
{
}

// Load assets
bool ModuleEnemies::Start()
{
	LOG("Loading array of enemies");

	//Set graphics.
	graphics = App->textures->Load("_png_Longmoan.png");

	//Set Up LongMoan Array
	for (int i = 0; i < NUM_OF_ENEMIES; i++)
	{
		LongMoan[i] = new ModuleLongmoan();
	}

	//Set start position to enemies.
	LongMoan[0]->position = { 292, UPPER_PATH };
	LongMoan[1]->position = { 400, MIDDLE_PATH };
	LongMoan[2]->position = { 472, UPPER_PATH };
	LongMoan[3]->position = { 496, MIDDLE_PATH };
	LongMoan[4]->position = { 736, MIDDLE_PATH };
	LongMoan[5]->position = { 816, UPPER_PATH };
	LongMoan[6]->position = { 1152, MIDDLE_PATH };
	LongMoan[7]->position = { 1232, UPPER_PATH };
	LongMoan[8]->position = { 1392, UPPER_PATH };
	LongMoan[9]->position = { 1392, LOWER_PATH };
	LongMoan[10]->position = { 1500, MIDDLE_PATH };
	LongMoan[11]->position = { 1500 - 8, UPPER_PATH  - 8};

	LongMoan[10]->SetBoss();
	LongMoan[11]->SetBoss();
	return true;

}


update_status ModuleEnemies::Update()
{
	
	for (int i = 0; i < NUM_OF_ENEMIES; i++)
	{
		if (LongMoan[i]->state != ENEMY_OUTOFSCREEN)
		{
			if (!stopEnemies)	// allways false till player is in PLAYER_MAGIC state
			{
				// This only happens when both are false. Just at the end of the magic attack 
				if (stopEnemies == startEnemies)				
					LongMoan[i]->livePoints -= (App->player->potions * 2);  //substract hit points to each enemy

				if (LongMoan[i]->Update() != UPDATE_CONTINUE) // enemies do whatever they do
					return UPDATE_ERROR;
			}
			else	// PLAYER_MAGIC
			{
				startEnemies = !stopEnemies;
				if (!LongMoan[i]->CompareAnimations(ENEMY_DEAD_ANIMATION))		// dead enemies remain dead 
				{
					LongMoan[i]->SetAnimation(ENEMY_MAGIC_HURT_ANIMATION);		 // alive enemies are under magic attack
				}
			}
		}
		else //enemies out of screen
		{
			// screen 0 relative + width of screen adding enemy width 
			int xMin = App->scene_lvl1->screenXZero + SCREEN_WIDTH + COLLIDER_ENEMY_WIDTH; 
			//new enemies come in screen
			if (LongMoan[i]->position.x < xMin   && LongMoan[i]->state == ENEMY_OUTOFSCREEN)
				if (LongMoan[i]->GetType() == NORMAL)
					LongMoan[i]->state = ENEMY_IDLE;
				else
					LongMoan[i]->state = ENEMY_WAIT;
		}
		if (LongMoan[i]->GetType() == BOSS && LongMoan[i]->state == ENEMY_WAIT)
			if (App->player->position.x > 1400)
				LongMoan[i]->state = ENEMY_IDLE;
	}
	

	// This only happens just at the end of magic attack 
	if (stopEnemies == startEnemies)		
	{
		startEnemies = true;
		App->player->potions = 0;
	}

	return UPDATE_CONTINUE;
}

// UnLoad assets
bool ModuleEnemies::CleanUp()
{
	if (LongMoan[0] == nullptr)
		return true;

	LOG("Unloading array of enemies");

	for (int i = 0; i < NUM_OF_ENEMIES; i++)
	{
		if (LongMoan[i] != nullptr)
		{
			delete LongMoan[i];
			LongMoan[i] = nullptr;
		}
	}
	
	App->textures->Unload(graphics);

	return true;
}

Animation* ModuleEnemies::GetCurrentAnimation(int i)
{
	return LongMoan[i]->GetCurrentAnimation();	
};

bool ModuleEnemies::checkGameFinished()
{
	for (int i = 0; i < NUM_OF_ENEMIES; ++i)
		if (LongMoan[i]->state != ENEMY_DEAD)
			return false;
	return true;
};