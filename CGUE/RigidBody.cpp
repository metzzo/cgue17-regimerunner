#include "RigidBody.h"
#include "Transformation.h"
#include "Pass.h"

namespace Engine {
	const RigidBody RigidBodyClass;

	RigidBody::RigidBody()
	{
		staticState = false;
		actor = nullptr;
		physicsMaterial = nullptr;
		shape = nullptr;
		staticFriction = 0;
		dynamicFriction = 0;
		restitution = 0;
	}


	RigidBody::~RigidBody()
	{
	}

	void RigidBody::TransformationUpdated()
	{

	}

	void RigidBody::SetStaticness(bool staticness)
	{
		assert(this->actor == nullptr);
		staticState = staticness;
	}

	bool RigidBody::IsStatic() const
	{
		return staticState;
	}

	PxRigidActor *RigidBody::GetActor() const
	{
		return actor;
	}

	void RigidBody::SetGeometry(PxGeometry* geometry)
	{
		if (this->actor != nullptr)
		{
			this->actor->release();
		}

		if (physicsMaterial == nullptr)
		{
			this->physicsMaterial = GetEngine()->GetPhysics()->createMaterial(staticFriction, dynamicFriction, restitution);
		}

		if (IsStatic())
		{
			this->actor = GetEngine()->GetPhysics()->createRigidStatic(PxTransform(GetTransformation()->GetPhysicMatrix()));
		}
		else
		{
			auto body = GetEngine()->GetPhysics()->createRigidDynamic(PxTransform(GetTransformation()->GetPhysicMatrix()));
			PxRigidBodyExt::updateMassAndInertia(*body, density);
			this->actor = body;
		}
		this->actor->userData = this;

		this->shape = GetActor()->createShape(*geometry, *physicsMaterial);

		GetEngine()->GetPhysicsScene()->addActor(*this->actor);
	}

	void RigidBody::SetMaterial(float staticFriction, float dynamicFriction, float restitution)
	{
		this->staticFriction = staticFriction;
		this->dynamicFriction = dynamicFriction;
		this->restitution = restitution;

		if (this->physicsMaterial != nullptr)
		{
			this->physicsMaterial->release();
			this->physicsMaterial = nullptr;
		}
	}

	void RigidBody::SetDensity(float density)
	{
		this->density = density;
	}

	float RigidBody::GetDensity() const
	{
		return density;
	}

	PxShape* RigidBody::GetShape() const
	{
		return this->shape;
	}
}
