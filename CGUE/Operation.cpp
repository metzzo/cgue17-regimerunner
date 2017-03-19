#include "Operation.h"
#include <cassert>


namespace Engine {
	Operation::Operation(Component *component)
	{
		assert(component != nullptr);
		this->component = component;
		this->pass = nullptr;
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

	void Operation::SetPass(Pass* pass)
	{
		this->pass = pass;
	}
}
