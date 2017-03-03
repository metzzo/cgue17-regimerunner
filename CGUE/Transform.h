#pragma once

#include "Eigen/Dense"
#include "Component.h"

using namespace Eigen;

namespace Engine {
	class Transform : public Component

	{
		Matrix4d transform;

	public:
		Transform();
		~Transform();
	};
}
