#pragma once
#include "Operation.h"
#include "HeightMapResource.h"
#include "RigidBody.h"
#include "Player.h"
#include "Model.h"
#include "CollectingPlaceBehaviour.h"

namespace Game {

	class WoodInteractionOperation : public Engine::Operation {
	public:
		explicit WoodInteractionOperation(Engine::Component* component) : Operation(component) {
		}

		void Execute() override;

	};

	class WoodInteraction : public Engine::Component {
		friend WoodInteractionOperation;

		Engine::RigidBody *rigidBody;
		Player* player;
		Engine::Model *model;
		bool placed;
		physx::PxDistanceJoint* joint;
	public:
		void Init() override;
		void Wire() override;
		explicit WoodInteraction(Player *player, Engine::Model *model);

		void Place(CollectingPlaceBehaviour* component);
		void Reset();
	};

}
