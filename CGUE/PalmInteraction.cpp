#include "PalmInteraction.h"
#include "Pass.h"
#include "Entity.h"
#include <SDL.h>
#include <iostream>
#include "Transformation.h"
#include "GameEngine.h"
#include "glm/gtc/matrix_transform.inl"
#include "RigidBody.h"
#include "CapsuleShape.h"
#include "Component.h"
#include "Camera.h"

namespace Game {

	void PalmInteractionOperation::Execute() {
		auto component = static_cast<PalmInteraction*>(this->GetComponent());
		auto spaceBar = component->GetEngine()->KeyDown(SDL_SCANCODE_SPACE);

		if (spaceBar) {
			auto posPalm = component->GetTransformation()->GetAbsolutePosition();
			auto posCamera = component->GetEngine()->GetMainCamera()->GetTransformation()->GetAbsolutePosition();
			auto dist = glm::distance(posPalm, posCamera);

			if (dist < 50.0) {
				std::cout << dist << std::endl;
				auto shape = new Engine::CapsuleShape(1, 20);
				component->GetEntity()->Add(shape);

				auto rigidBody = new Engine::RigidBody();
				rigidBody->SetStaticness(false);
				rigidBody->SetDensity(10);
				rigidBody->SetMaterial(0.5, 0.5, 0.5);
				component->GetEntity()->Add(rigidBody);
			}									
		}
	}

	void PalmInteraction::Init() {
		GetEngine()->GetUpdatePass()->AddOperation(new PalmInteractionOperation(this));
	}
}
