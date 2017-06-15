#pragma once
#include "Component.h"
#include "Frustum.h"
#include <foundation/Px.h>
#include "Operation.h"
#include <PxActor.h>
#include "CameraMovement.h"

namespace Engine {
	class RigidBody;
}

namespace Game {
	class PlayerOperation : public Engine::Operation
	{

	public:
		explicit PlayerOperation(Engine::Component* component)
			: Operation(component)
		{
		}

		void Execute() override;
	};

	class Player : public Engine::Component
	{
		friend PlayerOperation;

		double blood;
		CameraMovement *cameraMovement;
	public:
		Player();
		~Player();
		Engine::AABox GetAABB() const;
		physx::PxActor *GetActor() const;
		static physx::PxF32 GetHeight();
		static physx::PxF32 GetRadius();

		void Init() override;
		void Wire() override;
		void HelicopterHit();
	};
	extern const Player PlayerClass;
}

