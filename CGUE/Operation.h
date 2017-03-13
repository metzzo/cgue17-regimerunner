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

		virtual bool Execute() = 0;

		virtual QUEUE_TYPE GetQueueType() = 0;

		Component *GetComponent() const;
	};
}
