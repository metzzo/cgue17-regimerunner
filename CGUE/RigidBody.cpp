#include "RigidBody.h"
#include "Transformation.h"
#include "Pass.h"
#include "PxPhysicsAPI.h"
#include "GameEngine.h"

namespace Engine {
	const RigidBody RigidBodyClass;

	RigidBody::RigidBody()
	{
		staticState = false;
		actor = nullptr;
		physicsMaterial = nullptr;
		shape = nullptr;
		geometry = nullptr;
		staticFriction = 0;
		dynamicFriction = 0;
		restitution = 0;
		density = 10;
		localPose = PxTransform(PxIdentity);
	}


	RigidBody::~RigidBody()
	{
	}

	void RigidBody::TransformationUpdated()
	{

	}

	void RigidBody::SetStaticness(bool staticness)
	{
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
		this->geometry = geometry;

		if (this->actor != nullptr)
		{
			GetEngine()->GetPhysicsScene()->removeActor(*this->actor);
			this->actor->release();
		}

		if (physicsMaterial == nullptr)
		{
			this->physicsMaterial = GetEngine()->GetPhysics()->createMaterial(staticFriction, dynamicFriction, restitution);
		}

		PxRigidDynamic* body = nullptr;
		if (IsStatic())
		{
			this->actor = GetEngine()->GetPhysics()->createRigidStatic(PxTransform(GetTransformation()->GetPhysicMatrix()));
		}
		else
		{
			body = GetEngine()->GetPhysics()->createRigidDynamic(PxTransform(GetTransformation()->GetPhysicMatrix()));
			this->actor = body;
		}
		this->actor->userData = this;

		this->shape = GetActor()->createShape(*geometry, *physicsMaterial);
		this->shape->setLocalPose(localPose);

		if (body)
		{

			PxRigidBodyExt::updateMassAndInertia(*body, density);
		}

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

	void RigidBody::SetLocalPose(PxTransform transform)
	{
		this->localPose = transform;
	}

	float RigidBody::GetDensity() const
	{
		return density;
	}

	PxShape* RigidBody::GetShape() const
	{
		return this->shape;
	}

	void RigidBody::Refresh()
	{
		assert(this->geometry != nullptr);
		SetGeometry(this->geometry);
	}
}
