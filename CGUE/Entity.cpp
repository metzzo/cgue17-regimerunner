#include "Entity.h"
#include <typeinfo>
#include <iostream>

using namespace std;

namespace Engine {
	Entity::Entity()
	{
		
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

	Component* Entity::AddComponent(Component* component)
	{
		component->entity = this;
		this->components.push_back(component);
		return component;
	}

	Renderer* Entity::AddRenderer(Renderer* renderer)
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

	void Entity::Update(GameEngine* gameEngine)
	{
		for (auto &component : this->components)
		{
			component->Update(gameEngine);
		}
	}

	void Entity::Render(GameEngine* gameEngine)
	{
		for (auto &renderer : this->renderer)
		{
			renderer->Render(gameEngine);
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

	void Entity::Init(GameEngine* gameEngine)
	{
		for (auto &component : this->components)
		{
			component->Init(gameEngine);
		}

		for (auto &renderer : this->renderer)
		{
			renderer->Init(gameEngine);
		}
	}
}
