#include "WorldInteraction.h"
#include "Pass.h"
#include "Entity.h"
#include <SDL.h>
#include <iostream>
#include "Palm.h"
#include "Transformation.h"
#include "Camera.h"
#include "glm/gtc/matrix_transform.inl"

namespace Game {

	void SpaceBarPressed::Execute() {
		auto component = static_cast<WorldInteraction*>(this->GetComponent());
		auto spaceBar = component->GetEngine()->KeyDown(SDL_SCANCODE_SPACE);

		if (spaceBar) {
			auto root = component->GetEngine()->GetRootEntity();
			auto children = root->GetChildren();

			for (int i = 0; i < children->size(); i++) {
				auto c = children->at(i);

				if (Palm* v = dynamic_cast<Palm*>(c)) {					
					auto posPalm = v->GetTransformation()->GetAbsolutePosition();
					auto posCamera = component->GetEngine()->GetMainCamera()->GetTransformation()->GetAbsolutePosition();
					auto dist = glm::distance(posPalm, posCamera);

					if (dist < 50.0) {
						std::cout << dist << std::endl;
						auto mat = v->GetTransformation()->GetRelativeMatrix();

						mat = glm::translate(mat, vec3(0.0f, -4.0f, 0.0f));
						v->GetTransformation()->SetRelativeMatrix(mat);
					}									
				}
			}
		}
	}

	void WorldInteraction::Init() {
		GetEngine()->GetUpdatePass()->AddOperation(new SpaceBarPressed(this));
	}
}