#include "Transform.h"

namespace Engine {
	Transform::Transform()
	{
		this->transform = Matrix4d::Identity();
	}


	Transform::~Transform()
	{
		
	}
}
