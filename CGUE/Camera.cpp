#include "Camera.h"
#include "Entity.h"


namespace Engine {
	Camera::Camera()
	{
	}


	Camera::~Camera()
	{
	}

	void Camera::Wire()
	{
		WIRE_COMPONENT(this->transform, TransformationClass);
	}
}
