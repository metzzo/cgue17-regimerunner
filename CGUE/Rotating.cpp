#include "Rotating.h"
#include "Pass.h"
#include "Transformation.h"
#include "glm/gtc/matrix_transform.inl"
#include <SDL.h>

namespace Game {
	void RotatingOperation::Execute()
	{
		auto component = this->GetComponent();

		auto direction_lr = component->GetEngine()->KeyDown(SDL_SCANCODE_LEFT) - component->GetEngine()->KeyDown(SDL_SCANCODE_RIGHT);
		auto direction_ud = component->GetEngine()->KeyDown(SDL_SCANCODE_UP) - component->GetEngine()->KeyDown(SDL_SCANCODE_DOWN);

		if (direction_lr || direction_ud) {
			auto mat = component->GetTransformation()->GetRelativeMatrix();
			//mat = glm::rotate(mat, 0.05f, vec3(1.0f * direction_lr, 0.0f, 1.0f*direction_ud));
			mat = glm::translate(mat, vec3(0.1f * direction_lr, 0.0f, 0.1f*direction_ud));
			component->GetTransformation()->SetRelativeMatrix(mat);
		}
	}


	void Rotating::Init()
	{
		GetEngine()->GetUpdatePass()->AddOperation(new RotatingOperation(this));
	}
}
