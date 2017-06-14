#include "BaseGeometry.h"

namespace Engine
{
	BaseGeometry::BaseGeometry()
	{
		geometry = nullptr;
	}

	BaseGeometry::~BaseGeometry()
	{
	}

	physx::PxGeometry* BaseGeometry::GetGeometry(RigidBody* body)
	{
		if (geometry == nullptr)
		{
			geometry = MakeGeometry(body);
		}
		return geometry;
	}

	physx::PxTransform BaseGeometry::GetLocalPose() const
	{
		return localPose;
	}

	void BaseGeometry::SetLocalPose(physx::PxTransform pose)
	{
		this->localPose = pose;
	}
}
