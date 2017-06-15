#pragma once
#include "Operation.h"
#include "HeightMapResource.h"
#include "RigidBody.h"

namespace Game {

	class PalmInteractionOperation : public Engine::Operation {
		bool positioned;
	public:
		explicit PalmInteractionOperation(Engine::Component* component) : Operation(component) {
			positioned = false;
		}

		void Execute() override;

	};

	class PalmInteraction : public Engine::Component {
		friend PalmInteractionOperation;

		Engine::RigidBody *rigidBody;
	public:
		void Init() override;
		void Wire() override;
		explicit PalmInteraction();
	};

}
