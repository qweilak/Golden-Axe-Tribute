#include "ModuleSword.h"
#include <list>
#include "Application.h"
#include "ModuleCollision.h"
#include "ModulePlayer.h"

Sword::~Sword()
{
	if (collider != nullptr)
		collider->to_delete = true;
}

bool Sword::Update()
{
	bool ret = true;

	if (life > 0)
	{
		if (timer.Read() > delay + life)
			ret = false;
	}

	if (collider != NULL)
		collider->rect = { position.x, position.y - COLLIDER_SWORD_HEIGHT / 2, COLLIDER_SWORD_WIDTH, COLLIDER_SWORD_HEIGHT };
	return ret;
}

//------------------------

ModuleSword::ModuleSword() : Module()
{}

ModuleSword::~ModuleSword()
{
}

update_status ModuleSword::PostUpdate()
{
	for (std::list<Sword*>::iterator it = active.begin(); it != active.end();)
	{
		Sword* s = *it;

		if (s->Update() == false)
		{
			RELEASE(s);
			it = active.erase(it);
		break;
		}
		++it;
	}

	return UPDATE_CONTINUE;
}

void ModuleSword::AddSword(const Sword& sword, float x, float y, COLLIDER_TYPE collider_type, Uint32 delay)
{
	if (collider_type != COLLIDER_PLAYER_SHOT && collider_type != COLLIDER_ENEMY_SHOT)
		return;

	Sword* s = new Sword(sword);
	s->timer.Start();
	s->position.x = x;
	s->position.y = y;
	s->delay = delay;

	s->collider = App->collision->AddCollider({ s->position.x, s->position.y, 0, 0 }, collider_type, this);
	
	active.push_back(s);
}

// Always destroy particles that collide
void ModuleSword::OnCollisionBegin(Collider* c1, Collider* c2)
{

	if (c1->type == COLLIDER_PLAYER_SHOT && App->player->GetState() == PLAYER_ATTACK)
			App->player->SetState( PLAYER_COMBO_1);

	for (std::list<Sword*>::iterator it = active.begin(); it != active.end(); ++it)
	{
		if ((*it)->collider == c1)
		{
			RELEASE(*it);
			it = active.erase(it);
			break;
		}
	}
}
