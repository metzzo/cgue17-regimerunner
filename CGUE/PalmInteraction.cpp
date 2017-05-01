#include "PalmInteraction.h"
#include "Pass.h"
#include "Entity.h"
#include <SDL.h>
#include <iostream>
#include "Transformation.h"
#include "Camera.h"
#include "glm/gtc/matrix_transform.inl"

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
				component->GetTransformation()->Translate(vec3(0, .1f, 0));
			}									
		}
	}

	void PalmInteraction::Init() {
		GetEngine()->GetUpdatePass()->AddOperation(new PalmInteractionOperation(this));
	}
}