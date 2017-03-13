#include "Rotating.h"

#include "Transformation.h"
#include "glm/gtc/matrix_transform.inl"
#include <SDL.h>

namespace Game {
	bool RotatingOperation::Execute()
	{
		auto component = this->GetComponent();

		auto direction_lr = component->GetEngine()->KeyDown(SDL_SCANCODE_LEFT) - component->GetEngine()->KeyDown(SDL_SCANCODE_RIGHT);
		auto direction_ud = component->GetEngine()->KeyDown(SDL_SCANCODE_UP) - component->GetEngine()->KeyDown(SDL_SCANCODE_DOWN);

		if (direction_lr || direction_ud) {
			auto mat = component->GetTransformation()->GetRelativeMatrix();
			mat = glm::rotate(mat, 0.05f, vec3(1.0f * direction_lr, 0.0f, 1.0f*direction_ud));
			component->GetTransformation()->SetRelativeMatrix(mat);
		}
		return true;
	}

	Engine::QUEUE_TYPE RotatingOperation::GetQueueType()
	{
		return Engine::QUEUE_UPDATE;
	}

	void Rotating::Init()
	{
		GetEngine()->AddOperation(new RotatingOperation(this));
	}
}
