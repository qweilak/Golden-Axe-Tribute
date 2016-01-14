#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleCollision.h"
#include "ModuleAudio.h"
#include "ModuleScenelvl1.h"
#include "SDL/include/SDL.h"
#include "ModuleLongmoan.h"
#include "ModuleEnemies.h"

ModulePlayer::ModulePlayer(bool start_enabled) : Module(start_enabled)
{
	// idle animation (just the ship)
	animationsArray[PLAYER_IDLE_ANIMATION].frames.push_back({ 118, 13, 44, 72 });
	
	// move upwards
	animationsArray[PLAYER_UP_ANIMATION].frames.push_back({ 190, 103, 32, 64 });
	animationsArray[PLAYER_UP_ANIMATION].frames.push_back({ 229, 103, 32, 64 });
	animationsArray[PLAYER_UP_ANIMATION].frames.push_back({ 269, 103, 32, 64 });
	animationsArray[PLAYER_UP_ANIMATION].frames.push_back({ 305, 103, 32, 64 });
	animationsArray[PLAYER_UP_ANIMATION].loop = true;
	animationsArray[PLAYER_UP_ANIMATION].speed = 0.1f;

	// move downwards && forwards
	animationsArray[PLAYER_FORWARD_ANIMATION].frames.push_back({ 2, 103, 32, 64 });
	animationsArray[PLAYER_FORWARD_ANIMATION].frames.push_back({ 35, 103, 32, 64 });
	animationsArray[PLAYER_FORWARD_ANIMATION].frames.push_back({ 71, 103, 32, 64 });
	animationsArray[PLAYER_FORWARD_ANIMATION].frames.push_back({ 106, 103, 32, 64 });
	animationsArray[PLAYER_FORWARD_ANIMATION].loop = true;
	animationsArray[PLAYER_FORWARD_ANIMATION].speed = 0.1f;
	flip = false;

	// attack_1 animation
	animationsArray[PLAYER_ATTACK_ANIMATION].frames.push_back({ 0, 278, 80, 73 });
	animationsArray[PLAYER_ATTACK_ANIMATION].frames.push_back({ 62, 278, 80, 73 });
	animationsArray[PLAYER_ATTACK_ANIMATION].frames.push_back({ 142, 278, 80, 73 });
	animationsArray[PLAYER_ATTACK_ANIMATION].frames.push_back({ 62, 278, 80, 73 });
	animationsArray[PLAYER_ATTACK_ANIMATION].frames.push_back({ 0, 278, 80, 73 });
	animationsArray[PLAYER_ATTACK_ANIMATION].loop = false;
	animationsArray[PLAYER_ATTACK_ANIMATION].speed = 0.25f;
	
	// combo_1 animation
	animationsArray[PLAYER_COMBO1_ANIMATION].frames.push_back({ 0, 278, 80, 73 });
	animationsArray[PLAYER_COMBO1_ANIMATION].frames.push_back({ 62, 278, 80, 73 });
	animationsArray[PLAYER_COMBO1_ANIMATION].frames.push_back({ 142, 278, 80, 73 });
	animationsArray[PLAYER_COMBO1_ANIMATION].frames.push_back({ 205, 278, 80, 73 });
	animationsArray[PLAYER_COMBO1_ANIMATION].loop = false;
	animationsArray[PLAYER_COMBO1_ANIMATION].speed = 0.25f;

	// combo_2 animation
	animationsArray[PLAYER_COMBO2_ANIMATION].frames.push_back({ 281, 276, 76, 75 });
	animationsArray[PLAYER_COMBO2_ANIMATION].frames.push_back({ 356, 276, 76, 75 });
	animationsArray[PLAYER_COMBO2_ANIMATION].frames.push_back({ 119, 10, 76, 75 });
	animationsArray[PLAYER_COMBO2_ANIMATION].loop = false;
	animationsArray[PLAYER_COMBO2_ANIMATION].speed = 0.125f;
	
	// jump animations
	animationsArray[PLAYER_PREJUMP_ANIMATION].frames.push_back({ 400, 357, 62, 79 });
	animationsArray[PLAYER_JUMP_UP_ANIMATION].frames.push_back({ 400, 459, 62, 79 });
	animationsArray[PLAYER_JUMP_DOWN_ANIMATION].frames.push_back({ 445, 457, 62, 79 });
	animationsArray[PLAYER_JUMP_ATTACK_ANIMATION].frames.push_back({ 520, 454, 62, 79 });

	//magic
	animationsArray[PLAYER_MAGIC_ATTACK_ANIMATION].frames.push_back({ 264, 0, 32, 85 });
	animationsArray[PLAYER_MAGIC_ATTACK_ANIMATION].frames.push_back({ 319, 0, 32, 85 });
	animationsArray[PLAYER_MAGIC_ATTACK_ANIMATION].loop = false;
	animationsArray[PLAYER_MAGIC_ATTACK_ANIMATION].speed = 0.125f;
	//Hurted
	animationsArray[PLAYER_HURT_ANIMATION].frames.push_back({ 13, 663, 36, 58 });

	//dead
	animationsArray[PLAYER_DEAD_ANIMATION].frames.push_back({ 53, 656, 80, 64 });
	animationsArray[PLAYER_DEAD_ANIMATION].frames.push_back({ 127, 656, 80, 64 });
	animationsArray[PLAYER_DEAD_ANIMATION].frames.push_back({ 205, 656, 80, 64 });
	animationsArray[PLAYER_DEAD_ANIMATION].loop = false;
	animationsArray[PLAYER_DEAD_ANIMATION].speed = 0.125f;

	//magic Explosions
	animationsArray[PLAYER_RUN_ANIMATION].frames.push_back({ 0, 201, 56, 57 });
	animationsArray[PLAYER_RUN_ANIMATION].frames.push_back({ 56, 201, 56, 57 });
	animationsArray[PLAYER_RUN_ANIMATION].frames.push_back({ 112, 201, 56, 57 });
	animationsArray[PLAYER_RUN_ANIMATION].frames.push_back({ 168, 201, 56, 57 });
	animationsArray[PLAYER_RUN_ANIMATION].speed = 0.125f;
	animationsArray[PLAYER_RUN_ANIMATION].loop = true;

	//magic Explosions
	animationsArray[PLAYER_MAGIC_EXPLOSIONS_ANIMATION].frames.push_back({ 422, 31, 32, 37 });
	animationsArray[PLAYER_MAGIC_EXPLOSIONS_ANIMATION].frames.push_back({ 466, 31, 32, 37 });
	animationsArray[PLAYER_MAGIC_EXPLOSIONS_ANIMATION].frames.push_back({ 507, 31, 32, 37 });
	animationsArray[PLAYER_MAGIC_EXPLOSIONS_ANIMATION].loop = true;

}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	graphics = App->textures->Load("_png_ax.png");	
	
	//setup Player
	state = PLAYER_IDLE;
	position = { SCREEN_WIDTH / 2, MIDDLE_PATH };
	
	current_animation = &animationsArray[PLAYER_IDLE_ANIMATION];
	offset.x = position.x - animationsArray[PLAYER_IDLE_ANIMATION].frames[0].w / 2; // position - half width
	
	float x = offset.x - COLLIDER_PLAYER_WIDTH / 2;
	float y = position.y - COLLIDER_PLAYER_HEIGHT / 2;

	collider = App->collision->AddCollider({ x, y, COLLIDER_PLAYER_WIDTH, COLLIDER_PLAYER_HEIGHT }, COLLIDER_PLAYER, this);

	sword.life = 125;

	fx[0] = App->audio->LoadFx("_fx-01.wav");
	fx[1] = App->audio->LoadFx("_fx-02.wav");
	fx[2] = App->audio->LoadFx("_fx-03.wav");
	fx[3] = App->audio->LoadFx("_fx-04.wav");

	lifePoints = 9;

	left = NULL;
	right = NULL;
	potions = 1;

	timeWindow.Start();
	timeMax = 0;
	speed = PLAYER_SPEED_WALK;
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	if (graphics == nullptr)
		return true;
	LOG("Unloading player");
	App->textures->Unload(graphics);
	graphics = nullptr;
	left = NULL;
	right = NULL;
	return true;
}

// Update
update_status ModulePlayer::Update()
{
	//position x min 0 of the screen + margin + half width of player collider
	int xMin = App->scene_lvl1->screenXZero + SCREEN_MARGIN + COLLIDER_PLAYER_WIDTH / 2;
	//position x max 0 of the screen + screen width - screen margin - half width of collider player
	int xMax = App->scene_lvl1->screenXZero + SCREEN_WIDTH - SCREEN_MARGIN - COLLIDER_PLAYER_WIDTH / 2;

	App->enemies->stopEnemies = false;

	if (triggerRun.Read() > 250)
		canRun = 0;

	//Inputs only availables if time are over timeMax. we can set timeMax on each state, for example jumping need more time than attacks. Also if the state is hurt or dead we can't do anything
	if (timeWindow.Read() > timeMax && (state != PLAYER_HURT && state != PLAYER_DEAD))
	{
		
		if ((App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && canRun == SDL_SCANCODE_RIGHT) || speed == PLAYER_SPEED_RUNNING)
			speed = PLAYER_SPEED_RUNNING;
		if ((App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && canRun == SDL_SCANCODE_LEFT) || speed == PLAYER_SPEED_RUNNING)
			speed = PLAYER_SPEED_RUNNING;

		if (state != PLAYER_JUMP)
		{	
			if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			{
				animationsArray[PLAYER_FORWARD_ANIMATION].loop = true;
				state = PLAYER_MOVE;
			}

			if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
			{
				//in this game forward and down animations are the same.
				canRun = 0;
				animationsArray[PLAYER_FORWARD_ANIMATION].loop = true;
				state = PLAYER_MOVE;
			}

			if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
			{
				canRun = 0;
				animationsArray[PLAYER_UP_ANIMATION].loop = true;
				state = PLAYER_MOVE;
			}

			if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
			{
				timeWindow.Stop();
				timeWindow.Start();
				//is avalable to jump running. So we don't set canRun to Zero
				state = PLAYER_JUMP;
				current_animation = &animationsArray[PLAYER_PREJUMP_ANIMATION];
				
				//the sprite is not mirrored.
				if (flip)
					offset.x = position.x - 36;
				else
					offset.x = position.x - 26;

			}

			if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN && potions > 0)
			{
				canRun = 0;
				state = PLAYER_MAGIC;
				timeAnim.Stop();
				timeAnim.Start();
				timeWindow.Stop();
				timeWindow.Start();
				flip = false;
				fx_played = false;
			}

		}
		
		if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		{
			PlayerState lastState = state;
			canRun = 0;
			fx_played = false;
			switch (lastState)
			{

			case PLAYER_COMBO_1:
				animationsArray[PLAYER_COMBO2_ANIMATION].Reset();
				state = PLAYER_COMBO_2;
				timeWindow.Stop();
				timeWindow.Start();
				break;
			case PLAYER_COMBO_2:
				animationsArray[PLAYER_COMBO1_ANIMATION].Reset();
				state = PLAYER_COMBO_1;
				timeWindow.Stop();
				timeWindow.Start();
				break;
			case PLAYER_JUMP:
				timeAnim.Stop();
				timeAnim.Start();
				current_animation = &animationsArray[PLAYER_JUMP_ATTACK_ANIMATION];
				state = PLAYER_JUMP;
				break;
			default:
				animationsArray[PLAYER_ATTACK_ANIMATION].Reset();
				animationsArray[PLAYER_COMBO1_ANIMATION].Reset();
				state = PLAYER_ATTACK;
				timeWindow.Stop();
				timeWindow.Start();
				break;
			}
			//Adding the sword collider , when we set x position  12 comes from sword.collider.w 

			float colliderXOffset = COLLIDER_PLAYER_WIDTH / 2 - (flip * (COLLIDER_PLAYER_WIDTH + COLLIDER_SWORD_WIDTH));
			float colliderYOffset = - COLLIDER_PLAYER_HEIGHT / 2;

			App->swords->AddSword(sword, position.x + colliderXOffset, position.y + colliderYOffset, COLLIDER_PLAYER_SHOT);
		}
	}

	switch (state)
	{
	case PLAYER_IDLE:

		if (timeWindow.Read() > 1250 && current_animation != &animationsArray[PLAYER_IDLE_ANIMATION])
		{
			current_animation = &animationsArray[PLAYER_IDLE_ANIMATION];
			offset.x = position.x - animationsArray[PLAYER_IDLE_ANIMATION].frames[0].w / 2; // position - half width
			timeWindow.Stop();
		}
		timeMax = 0;
		break;
	case PLAYER_MOVE:

		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT  && position.x  > xMin)
		{
			
			position.x -= speed;
			flip = true;
			if (speed == PLAYER_SPEED_WALK)
			{
				if (current_animation != &animationsArray[PLAYER_FORWARD_ANIMATION])
				{
					current_animation = &animationsArray[PLAYER_FORWARD_ANIMATION];
					animationsArray[PLAYER_FORWARD_ANIMATION].loop = true;
				}
				offset.x = position.x - animationsArray[PLAYER_FORWARD_ANIMATION].frames[0].w / 2; // position - half width
			}
			if (speed == PLAYER_SPEED_RUNNING)
			{
				if (current_animation != &animationsArray[PLAYER_RUN_ANIMATION])
				{
					current_animation = &animationsArray[PLAYER_RUN_ANIMATION];
					animationsArray[PLAYER_RUN_ANIMATION].loop = true;
				}
				offset.x = position.x - animationsArray[PLAYER_RUN_ANIMATION].frames[0].w / 2; // position - half width
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && position.x < xMax)
		{
			offset.x = position.x - animationsArray[PLAYER_FORWARD_ANIMATION].frames[0].w / 2; // position - half width

			position.x += speed;
			flip = false;

			if (speed == PLAYER_SPEED_WALK)
			{
				if (current_animation != &animationsArray[PLAYER_FORWARD_ANIMATION])
				{
					current_animation = &animationsArray[PLAYER_FORWARD_ANIMATION];
					animationsArray[PLAYER_FORWARD_ANIMATION].loop = true;
				}
				offset.x = position.x - animationsArray[PLAYER_FORWARD_ANIMATION].frames[0].w / 2; // position - half width
			}
			if (speed == PLAYER_SPEED_RUNNING)
			{
				if (current_animation != &animationsArray[PLAYER_RUN_ANIMATION])
				{
					current_animation = &animationsArray[PLAYER_RUN_ANIMATION];
					animationsArray[PLAYER_RUN_ANIMATION].loop = true;
				}
				offset.x = position.x - animationsArray[PLAYER_RUN_ANIMATION].frames[0].w / 2; // position - half width
			}
			if (position.x > xMin + SCREEN_WIDTH / 2 && xMax < App->scene_lvl1->worldXmax - SCREEN_MARGIN - COLLIDER_PLAYER_WIDTH / 2)
				App->renderer->camera.x -= speed * 2;
		}

		// y max == screen height
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT && position.y < YMAX)  
		{
			offset.x = position.x - animationsArray[PLAYER_FORWARD_ANIMATION].frames[0].w / 2; // position - half width
			if (canRun == 0)
				position.y += speed / 2; 
			else
				position.y += speed;
			if (current_animation != &animationsArray[PLAYER_FORWARD_ANIMATION])
			{
				current_animation = &animationsArray[PLAYER_FORWARD_ANIMATION];
				animationsArray[PLAYER_FORWARD_ANIMATION].loop = true;
			}
			
		}

		// y min == screen height - 80
		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT  && position.y > YMIN) 
		{
			offset.x = position.x - animationsArray[PLAYER_UP_ANIMATION].frames[0].w / 2; // position - half width
			if (canRun == 0)
				position.y -= speed / 2;
			else
				position.y -= speed;
			if (current_animation != &animationsArray[PLAYER_UP_ANIMATION])
			{
				current_animation = &animationsArray[PLAYER_UP_ANIMATION];
				animationsArray[PLAYER_UP_ANIMATION].loop = true;
			}
		}
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
		{
			speed = PLAYER_SPEED_WALK;
			current_animation = &animationsArray[PLAYER_IDLE_ANIMATION];
			offset.x = position.x - animationsArray[PLAYER_IDLE_ANIMATION].frames[0].w / 2; // position - half width
	
			if (canRun == 0)
			{
				canRun = SDL_SCANCODE_LEFT;
				triggerRun.Stop();
				triggerRun.Start();
			}else
				canRun = 0;
		}
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
		{
			speed = PLAYER_SPEED_WALK;
			current_animation = &animationsArray[PLAYER_IDLE_ANIMATION];
			offset.x = position.x - animationsArray[PLAYER_IDLE_ANIMATION].frames[0].w / 2; // position - half width
	
			if (canRun == 0)
			{
				canRun = SDL_SCANCODE_RIGHT;
				triggerRun.Stop();
				triggerRun.Start();
			}
			else
				canRun = 0;
		}

		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_IDLE && App->input->GetKey(SDL_SCANCODE_UP) == KEY_IDLE &&
			App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_IDLE && App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_IDLE)
		{
			state = PLAYER_IDLE;
			current_animation->loop = false;
			timeWindow.Stop();
			timeWindow.Start();
		}

		timeMax = 0;
		break;

	case PLAYER_ATTACK:
		OnAttack(&animationsArray[PLAYER_ATTACK_ANIMATION], fx[0]);

		if (flip)
			offset.x = position.x - 45;
		else
			offset.x = position.x - 35;
		break;

	case PLAYER_COMBO_1:

		if (flip)
			offset.x = position.x - 45;
		else
			offset.x = position.x - 35;

		OnAttack(&animationsArray[PLAYER_COMBO1_ANIMATION], fx[1]);
		break;

	case PLAYER_COMBO_2:
		
		if (flip)
			offset.x = position.x - 55;
		else
			offset.x = position.x - 21;
		
		OnAttack(&animationsArray[PLAYER_COMBO2_ANIMATION], fx[1]);
		break;
		
	case PLAYER_JUMP:
		// while jumping we cannot begin to run
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_IDLE && App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_IDLE)
			canRun = 0;

		if (fx_played == false)
		{
			fx_played = true;
			App->audio->PlayFx(fx[0]);
		}
		
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT  && position.x  > xMin)
		{
			position.x -= speed / 2;
			offset.x -= speed / 2;
			flip = true;
		}

		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && position.x < xMax)
		{
			position.x += speed / 2;
			offset.x += speed / 2;
			flip = false;

		}

		if (timeWindow.Read() > 125)
		{
			if (timeWindow.Read() < 625)
			{
				if (current_animation == &animationsArray[PLAYER_PREJUMP_ANIMATION] || timeAnim.Read() > 125)
					current_animation = &animationsArray[PLAYER_JUMP_UP_ANIMATION];
				offset.y -= speed;
			}
			else if (timeWindow.Read() < 1125)
			{
				if (current_animation == &animationsArray[PLAYER_JUMP_UP_ANIMATION] || timeAnim.Read() > 125)
					current_animation = &animationsArray[PLAYER_JUMP_DOWN_ANIMATION];
				offset.y += speed;
			}
			else
			{
				current_animation = &animationsArray[PLAYER_PREJUMP_ANIMATION];
				state = PLAYER_IDLE;
			}
		}

		timeMax = 300;
		break;
	case PLAYER_MAGIC:

			App->enemies->stopEnemies = true;		//stop all enemies animations and updates
			if (current_animation != &animationsArray[PLAYER_MAGIC_ATTACK_ANIMATION])
				current_animation = &animationsArray[PLAYER_MAGIC_ATTACK_ANIMATION];

			if (fx_played == false)
			{
				fx_played = true;
				App->audio->PlayFx(fx[2]);
			}
			timeMax = 3000;
			
			if (timeWindow.Read() > timeMax)
			{
				state = PLAYER_IDLE;
				current_animation = &animationsArray[PLAYER_IDLE_ANIMATION];
				offset.x = position.x - animationsArray[PLAYER_IDLE_ANIMATION].frames[0].w / 2; // position - half width
			}

		break;
	case PLAYER_HURT:
				current_animation = &animationsArray[PLAYER_HURT_ANIMATION];
				if (flip)
					offset.x = position.x - 21;
				else
					offset.x = position.x - 15;
				
				if (timeWindow.Read() > timeMax)
				{
					if (lifePoints <= 0)
					{
						timeAnim.Stop();
						timeWindow.Stop();
						state = PLAYER_DEAD;
						fx_played = false;
					}
					else
					{
						state = PLAYER_IDLE;
						current_animation = &animationsArray[PLAYER_IDLE_ANIMATION];
						offset.x = position.x - animationsArray[PLAYER_IDLE_ANIMATION].frames[0].w / 2; // position - half width
					}
				}
		break;
	case PLAYER_DEAD:
			//Input
			
			//Animation 
			current_animation = &animationsArray[PLAYER_DEAD_ANIMATION];
			offset.x = position.x - animationsArray[PLAYER_DEAD_ANIMATION].frames[0].w / 2; // position - half width
			timeMax = 0;					
		break;
	}

	if (state != PLAYER_JUMP || current_animation == &animationsArray[PLAYER_PREJUMP_ANIMATION])
		offset.y = position.y - current_animation->frames[0].h;
	
	// Update collider
	collider->SetPos(position.x - COLLIDER_PLAYER_WIDTH / 2, position.y - COLLIDER_PLAYER_HEIGHT / 2);
	return UPDATE_CONTINUE;
}

void ModulePlayer::OnCollisionBegin(Collider* a, Collider* other)
{

	if (other->type == COLLIDER_ENEMY_SHOT )
	{
		state = PLAYER_HURT;
		--lifePoints;
		timeMax = 750;
		timeWindow.Stop();
		timeWindow.Start();
	}

	if (state == PLAYER_MOVE && other->type == COLLIDER_ENEMY )
	{
		speed = PLAYER_SPEED_WALK;
		state = PLAYER_IDLE;
		timeWindow.Stop();
		timeWindow.Start();
	}
}

void ModulePlayer::OnAttack( Animation* anim, uint fx)
{
	if (fx_played == false)
	{
		fx_played = true;
		App->audio->PlayFx(fx);
	}

	if (current_animation != anim)
		current_animation = anim;

	timeMax = 250;

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_IDLE && timeWindow.Read() > 1125)
		state = PLAYER_IDLE;
}