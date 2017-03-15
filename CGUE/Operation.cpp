#include "Operation.h"
#include <cassert>


namespace Engine {
	Operation::Operation(Component *component)
	{
		assert(component != nullptr);
		this->component = component;
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
}
