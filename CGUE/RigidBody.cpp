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
		staticFriction = 0;
		dynamicFriction = 0;
		restitution = 0;
		density = 10;
	}


	RigidBody::~RigidBody()
	{
	}

	void RigidBody::TransformationUpdated()
	{

	}

	void RigidBody::Init()
	{
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
			this->actor = GetEngine()->GetPhysics()->createRigidStatic(physx::PxTransform(GetTransformation()->GetPhysicMatrix()));
		}
		else
		{
			body = GetEngine()->GetPhysics()->createRigidDynamic(physx::PxTransform(GetTransformation()->GetPhysicMatrix()));
			this->actor = body;
		}
		this->actor->userData = this;

		for (auto& geometry : geometries)
		{
			auto g = geometry->GetGeometry(this);

			this->shape = GetActor()->createShape(*g, *physicsMaterial);
			this->shape->setLocalPose(geometry->GetLocalPose());
		}


		if (body)
		{

			PxRigidBodyExt::updateMassAndInertia(*body, density);
		}

		GetEngine()->GetPhysicsScene()->addActor(*this->actor);
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

	void RigidBody::AddGeometry(BaseGeometry* geometry)
	{
		this->geometries.push_back(geometry);
	}

	vector<BaseGeometry*>& RigidBody::GetGeometries()
	{
		return this->geometries;
	}

	void RigidBody::Refresh()
	{
		Init();
	}
}
