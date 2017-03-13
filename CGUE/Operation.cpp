#include "Operation.h"


namespace Engine {
	Operation::Operation(Component *component)
	{
		assert(component != nullptr);
		this->component = component;
	}

	Operation::~Operation()
	{
	}

	Component* Operation::GetComponent() const
	{
		return this->component;
	}
}
