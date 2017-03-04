#include "Transformation.h"

namespace Engine {
	const Transformation TransformationClass;

	Transformation::Transformation()
	{
		this->transform = Matrix4d::Identity();
	}


	Transformation::~Transformation()
	{
		
	}
}
