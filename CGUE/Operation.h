#pragma once
#include "Camera.h"

namespace Engine {
	class GameLogicComponent;

	class Operation
	{
		Component *component;
	public:
		explicit Operation(Component *component);
		virtual ~Operation();

		virtual void Execute() = 0;

		virtual OPERATION_TYPE GetOperationType() = 0;

		Component *GetComponent() const;
	};
}
