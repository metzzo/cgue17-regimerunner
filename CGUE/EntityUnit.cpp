#include "EntityUnit.h"
#include "Entity.h"
#include "Transformation.h"
#include <typeinfo>

namespace Engine {
	EntityUnit::EntityUnit()
	{
		this->entity = nullptr;
	}


	EntityUnit::~EntityUnit()
	{
	}

	Entity* EntityUnit::GetEntity() const
	{
		return this->entity;
	}

	GameEngine* EntityUnit::GetEngine() const
	{
		return this->entity->GetEngine();
	}

	Transformation* EntityUnit::GetTransformation() const
	{
		return this->entity->GetTransformation();
	}

	void EntityUnit::Init()
	{
		// nothing \o/
	}

	void EntityUnit::Wire()
	{
		// nothing \o/
	}

	const char* EntityUnit::GetClassName() const
	{
		return typeid(*this).name();
	}
}