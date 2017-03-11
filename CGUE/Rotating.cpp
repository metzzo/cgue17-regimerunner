#include "Rotating.h"

#include "Transformation.h"
#include "glm/gtc/matrix_transform.inl"

namespace Game {
	Rotating::Rotating()
	{
	}


	Rotating::~Rotating()
	{
	}

	void Rotating::Update()
	{
		auto mat = this->GetTransformation()->GetRelativeMatrix();
		mat = glm::rotate(mat, 0.05f, vec3(1.1f,0.9f,0.5f));
		this->GetTransformation()->SetRelativeMatrix(mat);
	}
}
