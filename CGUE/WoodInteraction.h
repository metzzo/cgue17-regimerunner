#pragma once
#include "Operation.h"
#include "HeightMapResource.h"
#include "RigidBody.h"
#include "Player.h"
#include "Model.h"

namespace Game {

	class WoodInteractionOperation : public Engine::Operation {
		physx::PxDistanceJoint* joint;
	public:
		explicit WoodInteractionOperation(Engine::Component* component) : Operation(component) {
			this->joint = nullptr;
		}

		void Execute() override;

	};

	class WoodInteraction : public Engine::Component {
		friend WoodInteractionOperation;

		Engine::RigidBody *rigidBody;
		Player* player;
		Engine::Model *model;
	public:
		void Init() override;
		void Wire() override;
		explicit WoodInteraction(Player *player);
	};

}
