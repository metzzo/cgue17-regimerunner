#include "Entity.h"
#include <typeinfo>
#include <iostream>

using namespace std;

namespace Engine {
	Entity::Entity(GameEngine *gameEngine)
	{
		this->gameEngine = gameEngine;
	}


	Entity::~Entity()
	{
		for (auto &component : this->components)
		{
			delete component;
		}
		for (auto &renderer : this->renderer)
		{
			delete renderer;
		}
	}

	Component* Entity::Add(Component* component)
	{
		component->entity = this;
		this->components.push_back(component);
		return component;
	}

	Renderer* Entity::Add(Renderer* renderer)
	{
		renderer->entity = this;
		this->renderer.push_back(renderer);
		return renderer;
	}

	void Entity::WireUp(Component** target, const char *name)
	{
		for (auto &component : this->components)
		{
			if (strcmp(component->GetClassName(), name) == 0)
			{
				*target = component;
				return;
			}
		}

		cout << "Could not WireUp Component " << endl;
		exit(1);
	}

	void Entity::WireUp(Renderer** target, const char *name)
	{
		for (auto &renderer : this->renderer)
		{
			if (strcmp(renderer->GetClassName(), name) == 0)
			{
				*target = renderer;
				return;
			}
		}

		cout << "Could not WireUp Renderer " << endl;
		exit(1);
	}

	void Entity::Update()
	{
		for (auto &component : this->components)
		{
			component->Update();
		}
	}

	void Entity::Render()
	{
		for (auto &renderer : this->renderer)
		{
			renderer->Render();
		}
	}

	void Entity::Wire()
	{
		for (auto &component : this->components)
		{
			component->Wire();
		}

		for (auto &renderer : this->renderer)
		{
			renderer->Wire();
		}
	}

	void Entity::Init()
	{
		for (auto &component : this->components)
		{
			component->Init();
		}

		for (auto &renderer : this->renderer)
		{
			renderer->Init();
		}
	}
}
