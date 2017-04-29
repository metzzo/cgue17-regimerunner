#pragma once
#include "Component.h"
#include "Operation.h"

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
	public:
		RigidBody();
		~RigidBody();

		void SetStaticness(bool staticness);
		bool IsStatic() const;
		PxRigidActor *GetActor() const;
		void SetGeometry(PxGeometry* geometry);
		void SetMaterial(float staticFriction, float dynamicFriction, float restitution);
		void SetDensity(float density);
		float GetDensity() const;
		PxShape* GetShape() const;
	};

	extern const RigidBody RigidBodyClass;
}
