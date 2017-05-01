#pragma once
#include "Component.h"
#include "Operation.h"
#include <cstdlib>

namespace Game {
	class HelicopterBehaviourOperation : public Engine::Operation
	{
		int duration;
		int direction;
	public:
		void ChangeDirection();

		explicit HelicopterBehaviourOperation(Engine::Component* component)
			: Operation(component)
		{
			ChangeDirection();
		}

		void Execute() override;
	};


	class HelicopterBehaviour : public Engine::Component
	{
		friend HelicopterBehaviourOperation;
	public:
		void Init() override;
	};
}

