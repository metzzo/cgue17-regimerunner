#pragma once
#include "BaseGeometry.h"
namespace Engine {
	class CapsuleGeometry :
		public BaseGeometry
	{
		float radius;
		float halfHeight;
	public:
		CapsuleGeometry(float radius, float halfHeight);
		~CapsuleGeometry();

		PxGeometry *MakeGeometry(RigidBody *rigidBody) override;
	};
}