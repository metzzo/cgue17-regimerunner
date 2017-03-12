#include "Entity.h"
#include <typeinfo>
#include <iostream>
#include "Transformation.h"

using namespace std;

namespace Engine {
	Entity::Entity(GameEngine *gameEngine)
	{
		this->gameEngine = gameEngine;
		this->parent = nullptr;
		this->transformation = new Transformation();

		this->Add(this->transformation);
	}


	Entity::~Entity()
	{
		for (auto &child : this->children)
		{
			delete child;
		}

		for (auto &component : this->components)
		{
			delete component;
		}
		for (auto &renderer : this->renderer)
		{
			delete renderer;
		}
	}

	GameEngine* Entity::GetEngine() const
	{
		return this->gameEngine;
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

	Entity* Entity::CreateChild()
	{
		auto child = new Entity(this->gameEngine);
		child->parent = this;
		this->children.push_back(child);
		return child;
	}

	Entity* Entity::GetParent() const
	{
		return this->parent;
	}

	const vector<Entity*>* Entity::GetChildren() const
	{
		return &this->children;
	}

	Transformation* Entity::GetTransformation() const
	{
		return this->transformation;
	}

	void Entity::WireUp(Component** target, const char *name, bool force)
	{
		if (*target == nullptr) {
			for (auto &component : this->components)
			{
				if (strcmp(component->GetClassName(), name) == 0)
				{
					*target = component;
					return;
				}
			}
			if (force) {
				this->GetEngine()->RaiseEngineError("Could not WireUp Renderer " + string(name));
			}
		}
	}

	void Entity::WireUp(Renderer** target, const char *name, bool force)
	{
		if (*target == nullptr) {
			for (auto &renderer : this->renderer)
			{
				if (strcmp(renderer->GetClassName(), name) == 0)
				{
					*target = renderer;
					return;
				}
			}
			if (force) {
				this->GetEngine()->RaiseEngineError("Could not WireUp Renderer " + string(name));
			}
		}
	}

	void Entity::Update()
	{
		for (auto &component : this->components)
		{
			component->Update();
		}

		for (auto &child : this->children)
		{
			child->Update();
		}
	}

	void Entity::Render()
	{
		for (auto &renderer : this->renderer)
		{
			renderer->Render();
		}

		for (auto &child : this->children)
		{
			child->Render();
		}
	}

	void Entity::Wire()
	{
		// set Transformation of Entity
		this->WireUp(reinterpret_cast<Component**>(&this->transformation), TransformationClass.GetClassName());

		for (auto &component : this->components)
		{
			component->Wire();
		}

		for (auto &renderer : this->renderer)
		{
			renderer->Wire();
		}

		for (auto &child : this->children)
		{
			child->Wire();
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

		for (auto &child : this->children)
		{
			child->Init();
		}
	}
}
