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
	}

	GameEngine* Entity::GetEngine() const
	{
		return this->gameEngine;
	}

	Component* Entity::Add(Component* component)
	{
		component->entity = this;
		this->components.push_back(component);
		component->AttachedToEntity();
		return component;
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
				RaiseEngineError("Could not WireUp Component " + string(name));
			}
		}
	}

	void Entity::TransformationUpdated()
	{
		for (auto &component : this->components)
		{
			component->TransformationUpdated();
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

		for (auto &child : this->children)
		{
			child->Init();
		}
	}
}
