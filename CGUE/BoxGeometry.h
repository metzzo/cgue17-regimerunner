#pragma once
#include "BaseGeometry.h"
#include "glm/detail/type_vec3.hpp"

namespace Engine {
	class BoxGeometry :
		public BaseGeometry
	{
		glm::vec3 size;
	public:
		explicit BoxGeometry(glm::vec3 size);
		~BoxGeometry();

		PxGeometry *MakeGeometry(RigidBody *rigidBody) override;
	};
}
