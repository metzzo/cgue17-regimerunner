#include "Operation.h"
#include <cassert>


namespace Engine {
	Operation::Operation(Pass *pass, Component *component)
	{
		assert(component != nullptr);
		this->component = component;
		this->pass = pass;
	}

	Operation::~Operation()
	{
	}

	int Operation::GetPriority()
	{
		return 0;
	}

	Component* Operation::GetComponent() const
	{
		return this->component;
	}

	Pass* Operation::GetPass() const
	{
		return this->pass;
	}
}
