#pragma once
#include "Component.h"
#include <PxRigidActor.h>

using namespace physx;

namespace Engine {
	class RigidBody;
	class PxTransform;

	class BaseGeometry
	{
	protected:
		physx::PxTransform localPose;
		physx::PxGeometry *geometry;
	public:
		BaseGeometry();
		virtual ~BaseGeometry();

		physx::PxGeometry *GetGeometry(RigidBody *body);

		virtual physx::PxGeometry *MakeGeometry(RigidBody *body) = 0;

		physx::PxTransform GetLocalPose() const;

		void SetLocalPose(physx::PxTransform pose);
	};
}
