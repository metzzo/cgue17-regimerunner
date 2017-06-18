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
		auto selectButton = component->GetEngine()->KeyDownLastFrame(SDL_SCANCODE_E);

		if (selectButton && !component->placed) {
			if (component->player->GetWood() == nullptr) {
				auto posWood = component->GetTransformation()->GetAbsolutePosition();
				auto cam = component->GetEngine()->GetMainCamera();
				auto posCamera = cam->GetTransformation()->GetAbsolutePosition();
				auto dist = glm::distance(posWood, posCamera);

				if (dist < 60.0) {

					if (component->rigidBody->IsStatic()) {
						std::cout << dist << std::endl;
						component->GetTransformation()->Translate(vec3(0, 10, 0));
						component->model->SetCullingEnabled(false);
						component->rigidBody->SetStaticness(false);
						component->rigidBody->Refresh();

					}
					auto transform = PxTransform(
						PxVec3(60, 0, 0)
					);

					auto playerActor = component->player->GetActor();
					component->joint = PxDistanceJointCreate(
						*component->GetEngine()->GetPhysics(),
						component->rigidBody->GetActor(),
						transform,
						static_cast<PxRigidActor*>(playerActor),
						PxTransform(PxIdentity));
					component->joint->setMaxDistance(15.0f);
					component->joint->setDistanceJointFlag(PxDistanceJointFlag::eMAX_DISTANCE_ENABLED, true);

					component->player->SetWood(component);
				}
			} else if (component->joint != nullptr)
			{
				component->Reset();
			}
		}
	}

	WoodInteraction::WoodInteraction(Player *player, Engine::Model *model) : Component()
	{
		this->rigidBody = nullptr;
		this->model = model;
		this->player = player;
		this->placed = false;
		this->joint = nullptr;
	}

	void WoodInteraction::Place(CollectingPlaceBehaviour* component)
	{
		GetTransformation()->SetRelativeMatrix(translate(mat4(), component->GetTransformation()->GetAbsolutePosition() + vec3(0, 100, 0)));
		GetTransformation()->Rotate(90, vec3(0, 1, 0));
		rigidBody->Refresh();

		placed = true;

		player->increaseGatheredWood();

		Reset();
	}

	void WoodInteraction::Reset()
	{
		this->joint->release();
		this->joint = nullptr;
		this->player->SetWood(nullptr);
	}

	void WoodInteraction::Init() {
		
		GetEngine()->GetUpdatePass()->AddOperation(new WoodInteractionOperation(this));
	}

	void WoodInteraction::Wire()
	{
		WIRE_COMPONENT(this->rigidBody, Engine::RigidBodyClass);
	}
}
