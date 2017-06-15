#include "PalmInteraction.h"
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

namespace Game {

	void PalmInteractionOperation::Execute() {
		auto component = static_cast<PalmInteraction*>(this->GetComponent());
		auto spaceBar = component->GetEngine()->KeyDown(SDL_SCANCODE_E);

		if (spaceBar && joint == nullptr) {
			auto posPalm = component->GetTransformation()->GetAbsolutePosition();
			auto cam = component->GetEngine()->GetMainCamera();
			auto posCamera = cam->GetTransformation()->GetAbsolutePosition();
			auto dist = glm::distance(posPalm, posCamera);

			if (dist < 50.0 && component->rigidBody->IsStatic()) {
				std::cout << dist << std::endl;

				auto raycastDir = normalize(cam->GetLookAtVector() - posCamera);
				auto physicsPos = PxVec3(posCamera.x, posCamera.y, posCamera.z);
				auto physicsDir = PxVec3(raycastDir.x, raycastDir.y, raycastDir.z);
				auto maxDistance = 50.0f;
				PxRaycastBuffer hit;

				auto status = component->GetEngine()->GetPhysicsScene()->raycast(physicsPos, physicsDir, maxDistance, hit);
				if (status && hit.block.actor && hit.block.actor->userData == component->rigidBody || true)
				{
					component->rigidBody->SetStaticness(false);
					auto& geometries = component->rigidBody->GetGeometries();
					geometries.erase(geometries.begin() + 1);

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
				}
			}									
		}
	}

	PalmInteraction::PalmInteraction(Player *player) : Component()
	{
		this->rigidBody = nullptr;
		this->player = player;
	}

	void PalmInteraction::Init() {
		
		GetEngine()->GetUpdatePass()->AddOperation(new PalmInteractionOperation(this));
	}

	void PalmInteraction::Wire()
	{
		WIRE_COMPONENT(this->rigidBody, Engine::RigidBodyClass);
	}
}
