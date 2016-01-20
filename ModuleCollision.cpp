#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"

using namespace std;

ModuleCollision::ModuleCollision() : Module()
{
	debug = false;

	// Matrix is game specific, change it to adapt to your own needs
	// ... think of moving this outside ModuleCollision

	matrix[COLLIDER_COLLECTIBLE][COLLIDER_COLLECTIBLE] = false;
	matrix[COLLIDER_COLLECTIBLE][COLLIDER_CONTINUE] = false;
	matrix[COLLIDER_COLLECTIBLE][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_COLLECTIBLE][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_COLLECTIBLE][COLLIDER_PLAYER_SHOT] = false;
	matrix[COLLIDER_COLLECTIBLE][COLLIDER_ENEMY_SHOT] = false;

	matrix[COLLIDER_PLAYER][COLLIDER_COLLECTIBLE] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_CONTINUE] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER_SHOT] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_ENEMY_SHOT] = true;

	matrix[COLLIDER_ENEMY][COLLIDER_COLLECTIBLE] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_CONTINUE] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_PLAYER_SHOT] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_ENEMY_SHOT] = false;

	matrix[COLLIDER_PLAYER_SHOT][COLLIDER_COLLECTIBLE] = false;
	matrix[COLLIDER_PLAYER_SHOT][COLLIDER_CONTINUE] = false;
	matrix[COLLIDER_PLAYER_SHOT][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER_SHOT][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_PLAYER_SHOT][COLLIDER_PLAYER_SHOT] = false;
	matrix[COLLIDER_PLAYER_SHOT][COLLIDER_ENEMY_SHOT] = false;

	matrix[COLLIDER_ENEMY_SHOT][COLLIDER_COLLECTIBLE] = false;
	matrix[COLLIDER_ENEMY_SHOT][COLLIDER_CONTINUE] = false;
	matrix[COLLIDER_ENEMY_SHOT][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_ENEMY_SHOT][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_ENEMY_SHOT][COLLIDER_PLAYER_SHOT] = false;
	matrix[COLLIDER_ENEMY_SHOT][COLLIDER_ENEMY_SHOT] = false;

	matrix[COLLIDER_CONTINUE][COLLIDER_COLLECTIBLE] = false;
	matrix[COLLIDER_CONTINUE][COLLIDER_CONTINUE] = false;
	matrix[COLLIDER_CONTINUE][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_CONTINUE][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_CONTINUE][COLLIDER_PLAYER_SHOT] = false;
	matrix[COLLIDER_CONTINUE][COLLIDER_ENEMY_SHOT] = false;
}

// Destructor
ModuleCollision::~ModuleCollision()
{}

update_status ModuleCollision::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for(list<Collider*>::iterator it = colliders.begin(); it != colliders.end();)
	{
		if((*it)->to_delete == true)
		{
			for (list<DoubleColliders>::iterator it2 = collisions.begin(); it2 != collisions.end();)
			{
				if (it2->c1 == *it || it2->c2 == *it)
				{
					it2 = collisions.erase(it2);
				}
				else
					++it2;
			}
			RELEASE(*it);
			it = colliders.erase(it);
		}
		else it++;
	}

	return UPDATE_CONTINUE;
}

// Called before render is available
update_status ModuleCollision::Update()
{
	Collider* c1;
	Collider* c2;
	statusCollision.c1 = nullptr;
	statusCollision.c2 = nullptr;
	list<DoubleColliders>::iterator c1c2;

	for(list<Collider*>::iterator it = colliders.begin(); it != colliders.end();)
	{
		c1 = *it;

		for(list<Collider*>::iterator it2 = ++it; it2 != colliders.end(); ++it2)
		{
			c2 = *it2;

			if(c1->CheckCollision(c2->rect) == true)
			{
				if (matrix[c1->type][c2->type] && c1->callback)		// Collision begin?
					if (CheckStatus(c1, c2))						
						c1->callback->OnCollisionBegin(c1, c2);		
					else
						c1->callback->OnCollision(c1, c2);
				
				if (matrix[c2->type][c1->type] && c2->callback)
					if (CheckStatus(c2, c1))						// Collision begin?
						c2->callback->OnCollisionBegin(c2, c1); 	
					else
						c2->callback->OnCollision(c2, c1);
			}
			else
			{
				
				for (list<DoubleColliders>::iterator it3 = collisions.begin(); it3 != collisions.end();)
				{
					if (it3->c1 == c1 && it3->c2 == c2 || it3->c2 == c1 && it3->c1 == c2)
					{
						c1c2 = it3;
						statusCollision = *c1c2;
						(*it3).c1->callback->OnCollisionEnd(c1, c2);
						(*it3).c2->callback->OnCollisionEnd(c2, c1);
						it3 = collisions.erase(c1c2);
					}
					else
						++it3;
				}
			}
		}
	}

	return UPDATE_CONTINUE;
}

// Called before render is available
update_status ModuleCollision::PostUpdate()
{
	if(App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		debug = !debug;

	if(debug == true)
	{
		for(list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it)
		{
			Collider* col = *it;
			Uint8 alpha = 100;
			switch(col->type)
			{
				case COLLIDER_NONE:
				App->renderer->DrawQuad(col->rect, 255, 255, 255, alpha);
				break;
				case COLLIDER_PLAYER:
				App->renderer->DrawQuad(col->rect, 0, 255, 0, alpha);
				break;
				case COLLIDER_ENEMY:
				App->renderer->DrawQuad(col->rect, 255, 0, 0, alpha);
				break;
				case COLLIDER_PLAYER_SHOT:
				App->renderer->DrawQuad(col->rect, 255, 255, 0, alpha);
				break;
				case COLLIDER_ENEMY_SHOT:
				App->renderer->DrawQuad(col->rect, 0, 255, 255, alpha);
				break;
				case COLLIDER_CONTINUE:
				App->renderer->DrawQuad(col->rect, 0, 0, 255, alpha);
				break;
				case COLLIDER_COLLECTIBLE:
					App->renderer->DrawQuad(col->rect, 255, 0, 255, alpha);
			}
		}
	}

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleCollision::CleanUp()
{
	LOG("Freeing all colliders");

	for(list<Collider*>::reverse_iterator it = colliders.rbegin(); it != colliders.rend(); ++it)
		RELEASE(*it);

	colliders.clear();
	collisions.clear();
	return true;
}

Collider* ModuleCollision::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, Module* callback)
{
	Collider* ret = new Collider(rect, type, callback);
	colliders.push_back(ret);
	return ret;
}

bool ModuleCollision::CheckStatus(const Collider* c1, const Collider* c2)
{
	for (list<DoubleColliders>::iterator it = collisions.begin(); it != collisions.end(); ++it)
	{
		if (it->c1 == c1 && it->c2 == c2)		//collision registered?
			return false;
	}

	DoubleColliders c1c2;
	c1c2.c1 = c1;
	c1c2.c2 = c2;	
	collisions.push_back(c1c2);
	return true;
}

// -----------------------------------------------------

bool Collider::CheckCollision(SDL_Rect r) const
{
	return (rect.x < r.x + r.w &&
			rect.x + rect.w > r.x &&
			rect.y < r.y + r.h &&
			rect.h + rect.y > r.y);
}

