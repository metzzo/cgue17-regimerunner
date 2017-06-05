#pragma once

#include "Component.h"
#include "glm/glm.hpp"
#include <foundation/PxMat44.h>

using namespace glm;
using namespace physx;

namespace Engine {
	class Transformation : public Component
	{
		mat4x4 relativeTransform;
		mat4x4 absoluteTransform;

	public:
		Transformation();
		~Transformation();

		void AttachedToEntity() override;

		void SetRelativeMatrix(mat4x4 newRelativeMatrix);
		void UpdateAbsoluteMatrix();
		mat4x4 GetAbsoluteMatrix() const;
		mat4x4 GetRelativeMatrix() const;

		vec3 GetAbsolutePosition();
		vec3 GetRelativePosition();
		void Translate(vec3 move);
		void Scale(vec3 size);
		void Rotate(float angle, vec3 axis);
		PxMat44 GetPhysicMatrix() const;
		void UpdatePhysicsMatrix(PxMat44 transform);
	};

	extern const Transformation TransformationClass;
}
