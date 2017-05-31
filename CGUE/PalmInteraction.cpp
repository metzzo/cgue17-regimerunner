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
		auto spaceBar = component->GetEngine()->KeyDown(SDL_SCANCODE_E);

		if (!positioned)
		{
			positioned = true;
			auto y = component->map->GetHeightAt(component->x, component->z) - 4;
			component->GetTransformation()->Translate(vec3(component->x, y, component->z));
			component->rigidBody->Refresh();
		}

		if (spaceBar) {
			auto posPalm = component->GetTransformation()->GetAbsolutePosition();
			auto posCamera = component->GetEngine()->GetMainCamera()->GetTransformation()->GetAbsolutePosition();
			auto dist = glm::distance(posPalm, posCamera);

			if (dist < 50.0 && component->rigidBody->IsStatic()) {
				std::cout << dist << std::endl;
				component->rigidBody->SetStaticness(false);
				component->rigidBody->Refresh();
			}									
		}
	}

	PalmInteraction::PalmInteraction(int x, int z, Engine::HeightMapResource *map) : Component()
	{
		this->map = map;
		this->x = x;
		this->z = z;
		this->rigidBody = nullptr;
	}

	void PalmInteraction::Init() {
		
		GetEngine()->GetUpdatePass()->AddOperation(new PalmInteractionOperation(this));
	}

	void PalmInteraction::Wire()
	{
		WIRE_COMPONENT(this->rigidBody, Engine::RigidBodyClass);
	}
}
