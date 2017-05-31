#pragma once
#include "Component.h"
#include "Operation.h"
#include <cstdlib>
#include "SpotLight.h"
#include "RenderPass.h"

namespace Game {
	class HelicopterBehaviourOperation : public Engine::Operation
	{
		float duration;
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

		Engine::SpotLight *spotLight;
	public:
		void Init() override;
		void Wire() override;
	};
}

