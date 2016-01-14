#include "Application.h"
#include "ModuleAudio.h"
#include "ModuleCollision.h"
#include "ModuleFadeToBlack.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModulePlayer.h"
#include "ModuleScenelvl1.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleEnemies.h"
#include "ModuleLongmoan.h"
#include <algorithm>


Potion::~Potion()
{
	if (collider != nullptr)
		collider->to_delete = true;
}

ModuleScenelvl1::ModuleScenelvl1(bool start_enabled) : Module(start_enabled)
{
	background = NULL;
}

ModuleScenelvl1::~ModuleScenelvl1()
{}

// Load assets
bool ModuleScenelvl1::Start()
{
	LOG("Loading lvl1 scene");
	//Resources
	background = App->textures->Load("_png_background.png");
	hud = App->textures->Load("_png_HUD.png");
	App->audio->PlayMusic("_music-level-1.ogg", 1.0f);

	App->collision->Enable(); // enable before player
	App->player->Enable();
	App->enemies->Enable();
	
	//SetUP world max
	worldXmax = 476;

	//SetUp potions on scene
	potionRect = { 177, 0, 23, 33 };

	AddPotion(576, 130);
	AddPotion(586, 196);
	AddPotion(928, 186);
	AddPotion(992, 154);

	// Representations for magic power
	collider = App->collision->AddCollider({ 476 - (SCREEN_WIDTH  / 2), 146, SCREEN_WIDTH / 2, 96 }, COLLIDER_CONTINUE, this);

	// Explosions on SCREEN, not WORLD -> so it's HUD but is in 'player' png
	explosions[0].point = { 32 , 64};
	explosions[1].point = { 96 , 16};
	explosions[2].point = { 64, 176 };
	explosions[3].point = { 272, 96 };
	explosions[4].point = { 256, 16 };
	explosions[5].point = { 160, 128 };
	explosions[6].point = { 240, 184 };

	for (int i = 0; i < 7; ++i )
	{
		explosions[i].animation = App->player->GetAnimation(PLAYER_MAGIC_EXPLOSIONS_ANIMATION);  //explosions inside the ax-battler.png 
		explosions[i].animation.speed = (float) (rand() % 2 + 1) * 0.125; // speed for each explosion
	}

	// HUD	
	hud_lifebars = { 0, 0, 88, 16};
	hud_face = { 97, 0 , 16, 16};
	hud_magic = { 120, 25, 49, 24};

	return true;
}

//Add potions
void ModuleScenelvl1::AddPotion(float x, float y)
{
	Potion* p = new Potion();
	p->position = { x, y };
	p->collider = App->collision->AddCollider({ p->position.x, p->position.y, 23, 33 }, COLLIDER_COLLECTIBLE, this);
	potion.push_back(p);
}

// UnLoad assets
bool ModuleScenelvl1::CleanUp()
{
	LOG("Unloading space scene");

	vectorPcNpc.clear();
	
	for (std::vector<Potion*>::iterator it = potion.begin(); it != potion.end();)
	{
		RELEASE(*it);
		it = potion.erase(it);	
	}

	App->textures->Unload(background);
	App->textures->Unload(hud);
	App->audio->StopMusic();

	App->enemies->Disable();
	App->player->Disable();
	App->collision->Disable();

	return true;
}

// Update: draw background
update_status ModuleScenelvl1::Update()
{

	//Music of game finished  and reload intro --> Boss Dead
	if (App->enemies->checkGameFinished())
	{
		if (music_end)
		{
			App->audio->PlayMusic("_music_conclusion.ogg", 0, false);
			music_end = false;
		}else if (!App->audio->IsPLaying())
			App->fade->FadeToBlack(App->scene_intro, this, 0);
	}

	//Music of game over  and reload intro --> Player Dead
	if (App->player->GetState() == PLAYER_DEAD)
	{
		if (music_end)
		{
			App->audio->PlayMusic("_music-game-over.ogg", 0, false);
			music_end = false;
		}
		else if (!App->audio->IsPLaying())
			App->fade->FadeToBlack(App->scene_intro, this, 0);
	}

	//Relative 0 of the screen
	screenXZero = -App->renderer->camera.x / SCREEN_SIZE;
	
	//Scroll world if needed. Is not available while player is doing magic attac neither when player is on the right side of screen
	if (App->player->GetState() != PLAYER_MAGIC && App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_IDLE )
		if (screenXZero + SCREEN_WIDTH < worldXmax && App->player->position.x > screenXZero + SCREEN_WIDTH/2)
				App->renderer->camera.x -= 2;

	//Update vector of characters to draw
	vectorPcNpc.clear();

	for (int i = 0; i < NUM_OF_ENEMIES; i++)
	{
		// if enemies are out of screen we don't need to draw them.
		if (App->enemies->GetEnemy(i)->state != ENEMY_OUTOFSCREEN)
		{
			ObjectsToDraw npcToDraw;
			// the enemie's texture is in ModuleEnemies. We need to load it only once and do not load it for each enemy.
			npcToDraw.texture = App->enemies->GetGraphics();
			npcToDraw.x = App->enemies->GetEnemy(i)->GetOffset().x;
			npcToDraw.y = App->enemies->GetEnemy(i)->GetOffset().y;
			npcToDraw.positionY = App->enemies->GetEnemy(i)->position.y;
			
			npcToDraw.rect = &App->enemies->GetCurrentAnimation(i)->GetCurrentFrame();
			npcToDraw.flip = App->enemies->GetEnemy(i)->GetFlip();
			npcToDraw.scaleXY = App->enemies->GetEnemy(i)->scaleXY;
			vectorPcNpc.push_back(npcToDraw);
		}
	}

	// the player is allways in the screen.
	ObjectsToDraw playerToDraw;									
	playerToDraw.texture = App->player->graphics;
	playerToDraw.x = App->player->GetOffset().x;
	playerToDraw.y = App->player->GetOffset().y;
	playerToDraw.positionY = App->player->position.y;
	playerToDraw.rect = &App->player->GetCurrentAnimation()->GetCurrentFrame();
	playerToDraw.flip = App->player->GetFlip();

	vectorPcNpc.push_back(playerToDraw);

	//sort vector of characters
	std::sort(vectorPcNpc.begin(), vectorPcNpc.end());

	//set HUD
	hud_magic.y = 25 * App->player->potions;
	switch (App->player->lifePoints)
	{
	case 1: case 2: case 3:		// values 1..3
		hud_lifebars.y = 34;
		break;
	case 4: case 5: case 6:		// values 4..6
		hud_lifebars.y = 17;
		break;
	case 7: case 8: case 9:		// values 7..9
		hud_lifebars.y = 0;
		break;
	default:
		hud_lifebars.y = 221;
		break;
	}
	
	// Draw background
	App->renderer->Blit(background, 0, 0, NULL);

	//Draw potions
	for (vector<Potion*>::iterator it = potion.begin(); it != potion.end(); ++it)
			App->renderer->Blit(hud, (*it)->position.x, (*it)->position.y, &potionRect);
	
	// Draw player and npcs
	for (vector<ObjectsToDraw>::iterator it = vectorPcNpc.begin(); it != vectorPcNpc.end(); ++it)
		App->renderer->Blit(it->texture, it->x, it->y, it->rect, 1.0f, it->flip, it->scaleXY);

	// Draw explosions --> last draw. Always in FrontGround
	if (App->player->GetState() == PLAYER_MAGIC && App->player->GetTimeOfAnimation() > 250)
	{
		for (int i = 0; i < 7; ++i)
			App->renderer->Blit(App->player->graphics, explosions[i].point.x - App->renderer->camera.x / SCREEN_SIZE, explosions[i].point.y, &explosions[i].animation.GetCurrentFrame());
	}

	//HUD
	
	App->renderer->Blit(hud, 135, 0, &hud_magic, 0);
	App->renderer->Blit(hud, 88, 200, &hud_lifebars, 0);
	App->renderer->Blit(hud, 186, 200, &hud_face, 0);
	return UPDATE_CONTINUE;
}


void ModuleScenelvl1::OnCollision(Collider* c, Collider* other)
{

	//continue next zone
	if (c->type == COLLIDER_CONTINUE)
	{
		//count enemies alive on screen
		int numOfEnemies = 0;
		for (int i = 0; i < NUM_OF_ENEMIES; i++)
		{
			if (App->enemies->GetEnemy(i)->GetType() == NORMAL)
				if (App->enemies->GetEnemy(i)->state != ENEMY_OUTOFSCREEN && App->enemies->GetEnemy(i)->state != ENEMY_DEAD)
					numOfEnemies++;
		}

		//without enemies enable next zone of the stage
		if (numOfEnemies == 0)
		{
			if (worldXmax <= 1506) 
				worldXmax += 212; 
			collider->SetPos(collider->rect.x + 212, collider->rect.y);
		}
	}

	//pick up potion
	if (c->type == COLLIDER_COLLECTIBLE && App->player->potions < 4)
	{
		App->player->potions++;
		for (std::vector<Potion*>::iterator it = potion.begin(); it != potion.end(); ++it)
		{
			if ((*it)->collider == c)
			{
				RELEASE(*it);
				it = potion.erase(it);
				break;
			}
		}
	}
}