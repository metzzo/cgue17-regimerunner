#include "BaseShape.h"
#include "Entity.h"


namespace Engine {
	BaseShape::BaseShape()
	{
	}


	BaseShape::~BaseShape()
	{
	}

	void BaseShape::Wire()
	{
		WIRE_COMPONENT(this->rigidBody, RigidBodyClass);
	}
}
