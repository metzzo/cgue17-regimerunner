#pragma once
#include "Operation.h"

namespace Game {
	class RotatingOperation : public Engine::Operation
	{
	public:
		explicit RotatingOperation(Engine::Component* component)
			: Operation(component)
		{
		}

		void Execute() override;
		Engine::QUEUE_TYPE GetQueueType() override;
	};

	class Rotating : public Engine::Component
	{
		void Init() override;
	};
}
