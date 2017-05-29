#pragma once
#include "Operation.h"
#include "PxPhysicsAPI.h"
#include "SpotLight.h"
#include "Camera.h"

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
		bool jumpPress;
		vec3 jump;
	public:
		explicit CameraMovementKeyOperation(Engine::Component* component)
			: Operation(component)
		{
			jumpPress = false;
		}

		void Execute() override;
	};

	class CameraMovement : public Engine::Component
	{
		friend CameraMovementKeyOperation;
		friend CameraMovementMouseOperation;

		PxController* controller;
		Engine::Camera *camera;
		Engine::SpotLight* spotLight;
	public:
		explicit CameraMovement(Engine::SpotLight *spotLight);

		void Init() override;
		void Wire() override;
	};
}
