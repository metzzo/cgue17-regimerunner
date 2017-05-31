#pragma once
#include "Component.h"
#include "Operation.h"
#include <PxRigidActor.h>

using namespace physx;

namespace Engine {
	class RigidBody : public Component
	{
		bool staticState;
		PxRigidActor *actor;
		PxMaterial* physicsMaterial;
		PxShape *shape;
		float staticFriction;
		float dynamicFriction;
		float restitution;
		float density;
		PxGeometry* geometry;
		PxTransform localPose;
		float mass;
	public:
		RigidBody();
		~RigidBody();

		void TransformationUpdated() override;

		void SetStaticness(bool staticness);
		bool IsStatic() const;
		PxRigidActor *GetActor() const;
		void SetGeometry(PxGeometry* geometry);
		void SetMaterial(float staticFriction, float dynamicFriction, float restitution);
		void SetDensity(float density);
		void SetLocalPose(PxTransform transform);
		float GetDensity() const;
		PxShape* GetShape() const;

		void Refresh();
	};

	extern const RigidBody RigidBodyClass;
}
