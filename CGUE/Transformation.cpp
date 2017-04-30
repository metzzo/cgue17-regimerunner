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

	vec3 Transformation::GetRelativePosition()
	{
		return vec3(this->relativeTransform[3]);
	}


	void Transformation::Translate(vec3 move)
	{
		SetRelativeMatrix(translate(relativeTransform, move));
	}

	void Transformation::Scale(vec3 size)
	{
		SetRelativeMatrix(scale(relativeTransform, size));
	}

	void Transformation::Rotate(float angle, vec3 axis)
	{
		if (angle != 0.0f) {
			SetRelativeMatrix(rotate(relativeTransform, angle, axis));
		}
	}

	PxMat44 Transformation::GetPhysicMatrix() const
	{
		return PxMat44(&GetAbsoluteMatrix()[0][0]);
	}

	void Transformation::UpdatePhysicsMatrix(PxMat44 transform)
	{
		auto m = mat4x4(
			transform.column0.x, transform.column0.y, transform.column0.z, transform.column0.w,
			transform.column1.x, transform.column1.y, transform.column1.z, transform.column1.w, 
			transform.column2.x, transform.column2.y, transform.column2.z, transform.column2.w, 
			transform.column3.x, transform.column3.y, transform.column3.z, transform.column3.w
		);
		SetRelativeMatrix(m);
	}
}
