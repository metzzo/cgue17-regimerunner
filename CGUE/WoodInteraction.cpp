#include "WoodInteraction.h"
#include "Pass.h"
#include "Entity.h"
#include <SDL.h>
#include <iostream>
#include "Transformation.h"
#include "GameEngine.h"
#include "glm/gtc/matrix_transform.inl"
#include "RigidBody.h"
#include "CapsuleGeometry.h"
#include "Component.h"
#include "Camera.h"
#include "Player.h"
#include "RigidBody.h"
#include "Model.h"

namespace Game {

	void WoodInteractionOperation::Execute() {
		auto component = static_cast<WoodInteraction*>(this->GetComponent());
		auto spaceBar = component->GetEngine()->KeyDown(SDL_SCANCODE_E) || component->GetEngine()->KeyDown(SDL_SCANCODE_SPACE);

		if (spaceBar && component->player->GetWood() == nullptr) {
			auto posWood = component->GetTransformation()->GetAbsolutePosition();
			auto cam = component->GetEngine()->GetMainCamera();
			auto posCamera = cam->GetTransformation()->GetAbsolutePosition();
			auto dist = glm::distance(posWood, posCamera);

			if (dist < 50.0 && component->rigidBody->IsStatic()) {
				std::cout << dist << std::endl;

				component->model->SetCullingEnabled(false);
				component->rigidBody->SetStaticness(false);
				component->rigidBody->Refresh();

				auto playerActor = component->player->GetActor();
				joint = PxDistanceJointCreate(
					*component->GetEngine()->GetPhysics(),
					component->rigidBody->GetActor(),
					PxTransform(PxIdentity),
					static_cast<PxRigidActor*>(playerActor),
					PxTransform(PxIdentity));
				joint->setMaxDistance(20.0f);
				joint->setDistanceJointFlag(PxDistanceJointFlag::eMAX_DISTANCE_ENABLED, true);

				component->player->SetWood(component);
			}
									
		}
	}

	WoodInteraction::WoodInteraction(Player *player) : Component()
	{
		this->rigidBody = nullptr;
		this->model = nullptr;
		this->player = player;
	}

	void WoodInteraction::Init() {
		
		GetEngine()->GetUpdatePass()->AddOperation(new WoodInteractionOperation(this));
	}

	void WoodInteraction::Wire()
	{
		WIRE_COMPONENT(this->rigidBody, Engine::RigidBodyClass);
		WIRE_COMPONENT(this->model, Engine::ModelClass);
	}
}
