#pragma once
#include "Component.h"
#include "Frustum.h"
#include <foundation/Px.h>
#include "Operation.h"

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
	public:
		Player();
		~Player();
		Engine::AABox GetAABB() const;
		static physx::PxF32 GetHeight();
		static physx::PxF32 GetRadius();

		void Init() override;
		void HelicopterHit();
	};
	extern const Player PlayerClass;
}

