#pragma once
#include "Operation.h"

namespace Game {

	class PalmInteractionOperation : public Engine::Operation {
		
	public:
		explicit PalmInteractionOperation(Engine::Component* component) : Operation(component) {
		}

		void Execute();

	};

	class PalmInteraction : public Engine::Component {
		friend PalmInteractionOperation;

	public:
		void Init() override;
	};

}