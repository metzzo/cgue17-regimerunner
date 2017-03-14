#include "Entity.h"
#include "Transformation.h"
#include <typeinfo>

namespace Engine {
	Component::Component()
	{
		this->entity = nullptr;
	}


	Component::~Component()
	{
	}

	Entity* Component::GetEntity() const
	{
		return this->entity;
	}

	GameEngine* Component::GetEngine() const
	{
		return this->entity->GetEngine();
	}

	Transformation* Component::GetTransformation() const
	{
		return this->entity->GetTransformation();
	}

	void Component::AttachedToEntity()
	{
		// nothing \o/
	}

	void Component::Init()
	{
		// nothing \o/
	}

	void Component::Wire()
	{
		// nothing \o/
	}

	const char* Component::GetClassName() const
	{
		return typeid(*this).name();
	}
}