#pragma once
#include "Component.h"

namespace Engine {

	class Operation
	{
		Component *component;
	public:
		explicit Operation(Component *component);
		virtual ~Operation();

		virtual void Execute() = 0;
		virtual int GetPriority();
		virtual OPERATION_TYPE GetOperationType() = 0;

		Component *GetComponent() const;
	};
}
