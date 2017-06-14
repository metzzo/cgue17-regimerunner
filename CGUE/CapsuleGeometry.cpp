#include "CapsuleGeometry.h"
#include "RigidBody.h"

namespace Engine {
	CapsuleGeometry::CapsuleGeometry(float radius, float halfHeight)
	{
		this->radius = radius;
		this->halfHeight = halfHeight;
	}


	CapsuleGeometry::~CapsuleGeometry()
	{
	}

	PxGeometry *CapsuleGeometry::MakeGeometry(RigidBody* rigidBody)
	{
		return new PxCapsuleGeometry(radius, halfHeight);
	}
}