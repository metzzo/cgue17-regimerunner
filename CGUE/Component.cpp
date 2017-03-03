#include "Component.h"



namespace Engine {
	Component::Component()
	{
		this->entity = nullptr;
	}


	Component::~Component()
	{
	}

	Entity* Component::getEntity() const
	{
		return this->entity;
	}

	void Component::Update(GameEngine* gameEngine)
	{
		// nothing \o/
	}

	void Component::Init(GameEngine* gameEngine)
	{
		// nothing \o/
	}

	void Component::Wire()
	{
		// nothing \o/
	}
}
