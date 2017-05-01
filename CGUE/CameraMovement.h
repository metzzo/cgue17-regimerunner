#pragma once
#include "Operation.h"
#include "PxPhysicsAPI.h"
#include "SpotLight.h"

using namespace physx;

namespace Game {

	class CameraMovementMouseOperation : public Engine::Operation
	{
		float yaw;
		float pitch;
	public:
		explicit CameraMovementMouseOperation(Engine::Component* component)
			: Operation(component)
		{
			yaw = 0;
			pitch = 0;
		}

		void Execute() override;
	};

	class CameraMovementKeyOperation : public Engine::Operation
	{
	public:
		explicit CameraMovementKeyOperation(Engine::Component* component)
			: Operation(component)
		{
		}

		void Execute() override;
	};

	class CameraMovement : public Engine::Component
	{
		friend CameraMovementKeyOperation;
		friend CameraMovementMouseOperation;

		PxController* controller;
		Engine::Camera *camera;
	public:
		void Init() override;
		void Wire() override;
	};
}
