#include "Rotating.h"

#include "Transformation.h"
#include "glm/gtc/matrix_transform.inl"
#include <SDL.h>

namespace Game {
	Rotating::Rotating()
	{
	}


	Rotating::~Rotating()
	{
	}

	void Rotating::Update()
	{
		auto direction_lr = this->GetEngine()->KeyDown(SDL_SCANCODE_LEFT) - this->GetEngine()->KeyDown(SDL_SCANCODE_RIGHT);
		auto direction_ud = this->GetEngine()->KeyDown(SDL_SCANCODE_UP) - this->GetEngine()->KeyDown(SDL_SCANCODE_DOWN);

		if (direction_lr || direction_ud) {
			auto mat = this->GetTransformation()->GetRelativeMatrix();
			mat = glm::rotate(mat, 0.05f, vec3(1.0f * direction_lr, 0.0f, 1.0f*direction_ud));
			this->GetTransformation()->SetRelativeMatrix(mat);
		}
	}
}
