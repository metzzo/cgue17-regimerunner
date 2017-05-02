#include "CapsuleShape.h"


namespace Engine {
	CapsuleShape::CapsuleShape(float radius, float halfHeight)
	{
		this->radius = radius;
		this->halfHeight = halfHeight;
	}


	CapsuleShape::~CapsuleShape()
	{
	}

	void CapsuleShape::Init()
	{
		rigidBody->SetGeometry(new PxCapsuleGeometry(radius, halfHeight));
	}
}