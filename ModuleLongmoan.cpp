#include "ModuleLongmoan.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "ModuleEnemies.h"
#include "ModulePlayer.h"
#include "ModuleAudio.h"

ModuleLongmoan::ModuleLongmoan(bool start_enabled) : Module(start_enabled)
{
	
	Animation idle;
	Animation forward;
	Animation up;
	Animation hurt;
	Animation hurtMagic;
	Animation attack1;
	Animation dead;

	time.Start();
	randomTime = rand() % 100;
	collider = App->collision->AddCollider({ position.x - COLLIDER_ENEMY_WIDTH / 2, position.y, COLLIDER_ENEMY_WIDTH, COLLIDER_ENEMY_HEIGHT }, COLLIDER_ENEMY, this);
	//Animations

	//Idle
	animArraY[ENEMY_IDLE_ANIMATION].frames.push_back({ 182, 1, 48, 68 });

	//Hurt
	animArraY[ENEMY_HURT_ANIMATION].frames.push_back({ 0, 199, 48, 68 });

	//Move forward
	animArraY[ENEMY_MAGIC_HURT_ANIMATION].frames.push_back({ 182, 1, 48, 68 });
	animArraY[ENEMY_MAGIC_HURT_ANIMATION].frames.push_back({ 377, 1, 48, 68 });
	animArraY[ENEMY_MAGIC_HURT_ANIMATION].frames.push_back({ 425, 1, 48, 68 });
	animArraY[ENEMY_MAGIC_HURT_ANIMATION].loop = true;
	animArraY[ENEMY_MAGIC_HURT_ANIMATION].speed = 0.250f;

	//Move forward
	animArraY[ENEMY_FORWARD_ANIMATION].frames.push_back({ 182, 1, 48, 68 });
	animArraY[ENEMY_FORWARD_ANIMATION].frames.push_back({ 230, 1, 48, 68 });
	animArraY[ENEMY_FORWARD_ANIMATION].frames.push_back({ 278, 1, 48, 68 });
	animArraY[ENEMY_FORWARD_ANIMATION].frames.push_back({ 326, 1, 48, 68 });
	animArraY[ENEMY_FORWARD_ANIMATION].loop = true;
	animArraY[ENEMY_FORWARD_ANIMATION].speed = 0.050f;

	//Move up
	animArraY[ENEMY_UP_ANIMATION].frames.push_back({ 0, 0, 48, 68 });
	animArraY[ENEMY_UP_ANIMATION].frames.push_back({ 45, 0, 48, 68 });
	animArraY[ENEMY_UP_ANIMATION].frames.push_back({ 92, 0, 48, 68 });
	animArraY[ENEMY_UP_ANIMATION].frames.push_back({ 132, 0, 48, 68 });
	animArraY[ENEMY_UP_ANIMATION].loop = true;
	animArraY[ENEMY_UP_ANIMATION].speed = 0.050f;

	//attack1
	animArraY[ENEMY_ATTACK_ANIMATION].frames.push_back({ 0, 268, 74, 70 });
	animArraY[ENEMY_ATTACK_ANIMATION].frames.push_back({ 73, 268, 74, 70 });
	animArraY[ENEMY_ATTACK_ANIMATION].frames.push_back({ 145, 268, 74, 70 });
	animArraY[ENEMY_ATTACK_ANIMATION].frames.push_back({ 206, 268, 74, 70 });
	animArraY[ENEMY_ATTACK_ANIMATION].frames.push_back({ 281, 268, 74, 70 });
	animArraY[ENEMY_ATTACK_ANIMATION].loop = false;
	animArraY[ENEMY_ATTACK_ANIMATION].speed = 0.250f;

	//dead
	animArraY[ENEMY_RUN_ANIMATION].frames.push_back({ 0, 73, 56, 59 });
	animArraY[ENEMY_RUN_ANIMATION].frames.push_back({ 56, 73, 56, 59 });
	animArraY[ENEMY_RUN_ANIMATION].frames.push_back({ 112, 73, 56, 59 });
	animArraY[ENEMY_RUN_ANIMATION].frames.push_back({ 168, 73, 56, 59 });
	animArraY[ENEMY_RUN_ANIMATION].loop = true;
	animArraY[ENEMY_RUN_ANIMATION].speed = 0.125f;

	//dead
	animArraY[ENEMY_DEAD_ANIMATION].frames.push_back({ 155, 220, 64, 36 });
	animArraY[ENEMY_DEAD_ANIMATION].frames.push_back({ 227, 220, 64, 36 });
	animArraY[ENEMY_DEAD_ANIMATION].loop = false;
	animArraY[ENEMY_DEAD_ANIMATION].speed = 0.125f;

	sword.life = 125;
	fx[0] = App->audio->LoadFx("_fx-11.wav");
	fx[1] = App->audio->LoadFx("_fx-12.wav");

	InitLongmoan();
}


ModuleLongmoan::~ModuleLongmoan()
{

}

void ModuleLongmoan::InitLongmoan()
{
	current_animation = &animArraY[ENEMY_IDLE_ANIMATION];
	state = ENEMY_OUTOFSCREEN;
	target = position;
	collider->SetPos(position.x - COLLIDER_ENEMY_WIDTH / 2, position.y - COLLIDER_ENEMY_HEIGHT / 2);
	type = NORMAL;
	livePoints = 6;
	speed = LONGMOAN_SPEED_WALK;
}

void ModuleLongmoan::SetBoss()
{
	type = BOSS;
	livePoints = 12;
	scaleXY = BOSS_SCALE;
}

update_status ModuleLongmoan::Update()
{

	if (livePoints <= 0)
	{
		state = ENEMY_DEAD;
	}

	//Update collider

	

	if (state != ENEMY_DEAD && state != ENEMY_WAIT )
	{
		if (time.Read() > randomTime)
		{
			if (type == BOSS)
				randomTime = 125 + rand() % 250;
			else
				randomTime = 250 + rand() % 250;
			BestAction();
		}

		if (App->player->position.x > position.x)
			flip = true;
		else
			flip = false;
	}

	switch (state)
	{
	case ENEMY_IDLE:

		if (current_animation != &animArraY[ENEMY_IDLE_ANIMATION])
			current_animation = &animArraY[ENEMY_IDLE_ANIMATION];
		//TODO
	break;
	case ENEMY_WAIT:

		if (flip)
			flip = false;
		
		if (livePoints < 12)
				BestAction();
	break;
	case ENEMY_TARGETSIDE:

		if (App->player->right == this &&  position.x < App->player->position.x + COLLIDER_ENEMY_WIDTH)
		{
			if (App->player->position.y < position.y)
				target.y += speed;
			else
				target.y -= speed;
		}

		if (App->player->left == this &&  position.x > App->player->position.x - COLLIDER_ENEMY_WIDTH)
		{
			if (App->player->position.y < position.y)
				target.y += 2;
			else
				target.y -= 2;
		}
		// we can do move state too without break;
	case ENEMY_MOVE:

		if (target.x < position.x)
			position.x -= speed;
		if (target.x > position.x)
			position.x += speed;

		if (target.y > position.y)
			position.y += speed;

		if (target.y < position.y)
		{
			current_animation = &animArraY[ENEMY_UP_ANIMATION];
			position.y -= speed;
		}
		else
			current_animation = &animArraY[ENEMY_FORWARD_ANIMATION];

	break;
	case ENEMY_RUNNING:

		if (target.x > position.x)
			flip = false;
		else
			flip = true;

		if (target.x < position.x)
			position.x -= speed;

		if (target.x > position.x)
			position.x += speed;
		
		current_animation = &animArraY[ENEMY_RUN_ANIMATION];
		
	break;
	case ENEMY_HURT:
		if (current_animation != &animArraY[ENEMY_HURT_ANIMATION])
			current_animation = &animArraY[ENEMY_HURT_ANIMATION];
	break;
	case ENEMY_ATTACK:

		if (App->player->GetState() == PLAYER_JUMP)
			state == ENEMY_MOVE;
		else
		{
			if (current_animation != &animArraY[ENEMY_ATTACK_ANIMATION] && time.Read())
			{
				current_animation = &animArraY[ENEMY_ATTACK_ANIMATION];
				randomTime += 250; //when animation starts we need to save the time to complete it.
				if (fx_played == false)
				{
					fx_played = true;
					App->audio->PlayFx(fx[0]);
				}
				float colliderXOffset = - COLLIDER_ENEMY_WIDTH / 2 - COLLIDER_SWORD_WIDTH + (flip * (COLLIDER_ENEMY_WIDTH + COLLIDER_SWORD_WIDTH));
				float colliderYOffset = - COLLIDER_ENEMY_HEIGHT / 2 - 1;
				App->swords->AddSword(sword, position.x + colliderXOffset, position.y + colliderYOffset , COLLIDER_ENEMY_SHOT);
				
			}
		}
	break;
	case ENEMY_DEAD:

		if (current_animation != &animArraY[ENEMY_DEAD_ANIMATION])
		{
			current_animation = &animArraY[ENEMY_DEAD_ANIMATION];
			App->audio->PlayFx(fx[1]);
		}

		if (collider != NULL) 
		{
			if (App->player->left == this)
				App->player->left = NULL;
			if (App->player->right == this)
				App->player->right = NULL;

			collider->to_delete = true;
			collider = NULL;
		}
		
	break;
	case ENEMY_COMBO:
		//TODO
	break;
	}

	if (type == BOSS)
		offset = { position.x - (current_animation->frames[0].w / 2) * BOSS_SCALE, position.y - current_animation->frames[0].h * BOSS_SCALE };
	else
		offset = { position.x - (current_animation->frames[0].w / 2), position.y - current_animation->frames[0].h };

	if (collider != NULL)
		collider->SetPos ( position.x - COLLIDER_ENEMY_WIDTH / 2, position.y - COLLIDER_ENEMY_HEIGHT / 2 - 1 ); 
	
	return UPDATE_CONTINUE;
}

void ModuleLongmoan::BestAction()
{

	// --> Set enemy state idle
	time.Stop();
	time.Start();
	EnemyState lastState = state;
	state = ENEMY_IDLE;
	if (App->player->left == this)
		App->player->left = NULL;

	if (App->player->right == this)
		App->player->right = NULL;

	fx_played = false;
	speed = LONGMOAN_SPEED_WALK;;
	// Our enemy moves allways after attack
	if (lastState == ENEMY_ATTACK && App->player->GetState() == PLAYER_HURT)
	{	
		
		state = ENEMY_MOVE;
		return;
	}
	//select place to move on y axis
	if (App->player->position.y - 8 < position.y )
	{
		state = ENEMY_MOVE;
		target.y = App->player->position.y - 8;
	}

	if (App->player->position.y + 8 > position.y)
	{
		state = ENEMY_MOVE;
		target.y = App->player->position.y + 8;
	}

	// the enemy don't need to move on x if player is dead
	if (App->player->GetState() == PLAYER_DEAD)
		return;
	
	//If player is faraway from enemy 
	if (App->player->position.x - 100 > position.x && lastState != ENEMY_TARGETSIDE )
	{
		target.x = App->player->position.x - 100;
		state = ENEMY_MOVE;
	}

	if (App->player->position.x + 100 < position.x && lastState != ENEMY_TARGETSIDE)
	{
		target.x = App->player->position.x + 100;
		state = ENEMY_MOVE;
	}

	//If player is close to enemy 
	if (position.x - App->player->position.x <= 100 && App->player->position.x < position.x)
 	{
		if (App->player->right == NULL )
		{
			target.x = App->player->position.x + COLLIDER_ENEMY_WIDTH;
			state = ENEMY_TARGETSIDE;
			App->player->right = this;
		}
		else if (App->player->left == NULL )
		{
			target.x = App->player->position.x - COLLIDER_ENEMY_WIDTH;
			state = ENEMY_TARGETSIDE;
			App->player->left = this;
				
			if (App->player->position.y - 8 >= position.y || App->player->position.y + 8 <= position.y)
			{
					target.x = App->player->position.x - COLLIDER_ENEMY_WIDTH * 2;
					speed = LONGMOAN_SPEED_RUNNING;
					state = ENEMY_RUNNING;
					randomTime = 375;
			}
		}
		else
		{
			target.x = App->player->position.x + 100;
			state = ENEMY_TARGETSIDE;
		}

	}
	
	if (App->player->position.x - position.x <= 100 && App->player->position.x > position.x)
	{
		if (App->player->left == NULL )
		{
			target.x = App->player->position.x - COLLIDER_ENEMY_WIDTH;
			state = ENEMY_TARGETSIDE;
			App->player->left = this;
		}
		else if (App->player->right == NULL )
		{
			target.x = App->player->position.x + COLLIDER_ENEMY_WIDTH;
			state = ENEMY_TARGETSIDE;
			App->player->right = this;
				
			if (App->player->position.y - 8 >= position.y || App->player->position.y + 8 <= position.y)
			{
				target.x = App->player->position.x + COLLIDER_ENEMY_WIDTH * 2;
				speed = LONGMOAN_SPEED_RUNNING;
				state = ENEMY_RUNNING;
				randomTime = 375;
			}
		}
		else
		{
			target.x = App->player->position.x - 100;
			state = ENEMY_TARGETSIDE;
		}
	}


	// Melee range !!! Kill player
	if (state == ENEMY_TARGETSIDE && (App->player->position.y - position.y < 0 && App->player->position.y - position.y > - 6 ))
	{
		if (App->player->left == this && target.x == position.x)
		{
			state = ENEMY_ATTACK;
			animArraY[ENEMY_ATTACK_ANIMATION].Reset();
		}
			
		if (App->player->right == this && target.x == position.x)
		{
			state = ENEMY_ATTACK;
			animArraY[ENEMY_ATTACK_ANIMATION].Reset();
		}
			
	}

	return;
}

void ModuleLongmoan::OnCollisionBegin(Collider* c, Collider* other)
{
	if (state == ENEMY_DEAD)
		return;

	if (other->type == COLLIDER_PLAYER_SHOT)
	{
		time.Stop();
		time.Start();
		randomTime = 500 + rand() % 300;
		livePoints -= 1;			
		if (livePoints <= 0)
		{
			state = ENEMY_DEAD;
			if (App->player->GetState() != PLAYER_JUMP)
				App->player->SetState(PLAYER_IDLE);
		}
		else
			state = ENEMY_HURT;
	}
}
