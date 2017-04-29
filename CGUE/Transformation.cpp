#include "Transformation.h"
#include "Entity.h"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/string_cast.hpp"
#include <iostream>

namespace Engine {
	const Transformation TransformationClass;

	Transformation::Transformation()
	{
		this->relativeTransform = mat4x4(1.0f);
		this->absoluteTransform = mat4(1.0f);
	}


	Transformation::~Transformation()
	{
		
	}

	void Transformation::AttachedToEntity()
	{
		this->UpdateAbsoluteMatrix();
	}

	void Transformation::SetRelativeMatrix(mat4x4 newRelativeMatrix)
	{
		this->relativeTransform = newRelativeMatrix;
		this->UpdateAbsoluteMatrix();
	}

	void Transformation::UpdateAbsoluteMatrix()
	{
		if (this->GetEntity()->GetParent() != nullptr) {
			this->absoluteTransform = relativeTransform * this->GetEntity()->GetParent()->GetTransformation()->absoluteTransform;
		} else
		{
			this->absoluteTransform = relativeTransform;
		}

		this->GetEntity()->TransformationUpdated();

		auto children = this->GetEntity()->GetChildren();
		for (auto &child : *children)
		{
			child->GetTransformation()->UpdateAbsoluteMatrix();
		}
	}

	mat4x4 Transformation::GetAbsoluteMatrix() const
	{
		return this->absoluteTransform;
	}

	mat4x4 Transformation::GetRelativeMatrix() const
	{
		return this->relativeTransform;
	}

	vec3 Transformation::GetAbsolutePosition()
	{
		return vec3(this->absoluteTransform[3]);
	}

	void Transformation::Translate(vec3 move)
	{
		SetRelativeMatrix(translate(relativeTransform, move));
	}

	void Transformation::Scale(vec3 size)
	{
		SetRelativeMatrix(scale(relativeTransform, size));
	}

	PxTransform Transformation::GetPhysicPosition()
	{
		auto pos = GetAbsolutePosition();
		return PxTransform(pos.x, pos.y, pos.z);
	}
}
