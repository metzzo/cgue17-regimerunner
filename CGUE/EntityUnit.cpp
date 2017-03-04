#include "EntityUnit.h"
#include "Entity.h"
#include <typeinfo>

namespace Engine {
	EntityUnit::EntityUnit()
	{
		this->entity = nullptr;
	}


	EntityUnit::~EntityUnit()
	{
	}

	Entity* EntityUnit::getEntity() const
	{
		return this->entity;
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