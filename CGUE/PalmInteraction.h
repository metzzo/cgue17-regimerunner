#pragma once
#include "Operation.h"
#include "HeightMapResource.h"

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

		Engine::HeightMapResource *map;
		int x;
		int z;
	public:
		void Init() override;
		explicit PalmInteraction(int x, int z, Engine::HeightMapResource *map);
	};

}
