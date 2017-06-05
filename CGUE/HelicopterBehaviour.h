#pragma once
#include "Component.h"
#include "Operation.h"
#include <cstdlib>
#include "SpotLight.h"

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
		bool broken;
		Engine::Transformation *mainRotor;
		Engine::Transformation *sideRotor;
	public:
		explicit HelicopterBehaviour(Engine::Transformation *mainRotor, Engine::Transformation *sideRotor, bool broken);
		
		void Init() override;
		void Wire() override;
	};
}

