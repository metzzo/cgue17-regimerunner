#include "BoxGeometry.h"


namespace Engine {
	BoxGeometry::BoxGeometry(glm::vec3 size)
	{
		this->size = size;
	}

	BoxGeometry::~BoxGeometry()
	{
	}

	PxGeometry* BoxGeometry::MakeGeometry(RigidBody* rigidBody)
	{
		return new PxBoxGeometry(size.x/2, size.y/2, size.z/2);
	}
}
