#pragma once
#include "Component.h"
#include "Operation.h"
#include <PxRigidActor.h>
#include "BaseGeometry.h"
#include <vector>

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
		std::vector<BaseGeometry*> geometries;
	public:
		RigidBody();
		~RigidBody();

		void TransformationUpdated() override;
		void Init() override;

		void SetStaticness(bool staticness);
		bool IsStatic() const;
		PxRigidActor *GetActor() const;
		void SetMaterial(float staticFriction, float dynamicFriction, float restitution);
		void SetDensity(float density);
		float GetDensity() const;

		void AddGeometry(BaseGeometry *geometry);

		void Refresh();
	};

	extern const RigidBody RigidBodyClass;
}
