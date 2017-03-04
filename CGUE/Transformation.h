#pragma once

#include "Eigen/Dense"
#include "Component.h"

using namespace Eigen;

namespace Engine {
	class Transformation : public Component

	{
		Matrix4d transform;

	public:
		Transformation();
		~Transformation();
	};

	extern const Transformation TransformationClass;
}
