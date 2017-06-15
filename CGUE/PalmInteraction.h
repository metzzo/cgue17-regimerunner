#pragma once
#include "Operation.h"
#include "HeightMapResource.h"
#include "RigidBody.h"
#include "Player.h"

namespace Game {

	class PalmInteractionOperation : public Engine::Operation {
		physx::PxDistanceJoint* joint;
	public:
		explicit PalmInteractionOperation(Engine::Component* component) : Operation(component) {
			this->joint = nullptr;
		}

		void Execute() override;

	};

	class PalmInteraction : public Engine::Component {
		friend PalmInteractionOperation;

		Engine::RigidBody *rigidBody;
		Player* player;
	public:
		void Init() override;
		void Wire() override;
		explicit PalmInteraction(Player *player);
	};

}
