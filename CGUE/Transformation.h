#pragma once

#include "Component.h"
#include "glm/glm.hpp"

using namespace glm;

namespace Engine {
	class Transformation : public Component
	{
		mat4x4 relativeTransform;
		mat4x4 absoluteTransform;
		mat4x4 projectionViewTransform;

	public:
		Transformation();
		~Transformation();

		void AttachedToEntity() override;

		void SetRelativeMatrix(mat4x4 newRelativeMatrix);
		void UpdateAbsoluteMatrix();
		mat4x4 GetAbsoluteMatrix() const;
		mat4x4 GetRelativeMatrix() const;
	};

	extern const Transformation TransformationClass;
}
