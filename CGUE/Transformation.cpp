#include "Transformation.h"
#include "Entity.h"

namespace Engine {
	const Transformation TransformationClass;

	Transformation::Transformation()
	{
		this->relativeTransform = mat4x4(1.0f);
		this->absoluteTransform = mat4(1.0f);
		this->mvpTransform = mat4(1.0f);
	}


	Transformation::~Transformation()
	{
		
	}

	void Transformation::SetRelativeMatrix(mat4x4 newRelativeMatrix)
	{
		this->relativeTransform = newRelativeMatrix;
		this->UpdateAbsoluteMatrix();
	}

	void Transformation::UpdateAbsoluteMatrix()
	{
		this->absoluteTransform = relativeTransform * this->GetEntity()->GetParent()->GetTransformation()->absoluteTransform;
		this->mvpTransform = this->projectionViewTransform * this->absoluteTransform;

		auto children = this->GetEntity()->GetChildren();
		for (auto &child : *children)
		{
			child->GetTransformation()->UpdateAbsoluteMatrix();
		}
	}

	void Transformation::SetProjectionViewMatrix(mat4x4 projectionViewMatrix)
	{
		this->projectionViewTransform = projectionViewMatrix;
		this->mvpTransform = projectionViewMatrix * this->absoluteTransform;

		auto children = this->GetEntity()->GetChildren();
		for (auto &child : *children)
		{
			child->GetTransformation()->SetProjectionViewMatrix(projectionViewMatrix);
		}
	}

	mat4x4 Transformation::GetAbsoluteMatrix() const
	{
		return this->absoluteTransform;
	}

	mat4x4 Transformation::GetMvpMatrix() const
	{
		return this->mvpTransform;
	}

	mat4x4 Transformation::GetRelativeMatrix() const
	{
		return this->relativeTransform;
	}
}
