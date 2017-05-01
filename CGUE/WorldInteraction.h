#pragma once
#include "Operation.h"

namespace Game {

	class SpaceBarPressed : public Engine::Operation {
		
	public:
		explicit SpaceBarPressed(Engine::Component* component) : Operation(component) {
		}

		void Execute();

	};

	class WorldInteraction : public Engine::Component {
		friend SpaceBarPressed;
		PxController* controller;
		

	public:
		void Init() override;
	};

}