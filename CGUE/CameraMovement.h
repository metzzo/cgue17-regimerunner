#pragma once
#include "Operation.h"
#include "PxPhysicsAPI.h"
#include "SpotLight.h"
#include "Camera.h"
#include "Entity.h"

using namespace physx;

namespace Game {
	class Player;


	class CameraMovementOperation : public Engine::Operation
	{
		float yaw;
		float pitch;

		bool jumpPress;
		vec3 jump;
	public:
		explicit CameraMovementOperation(Engine::Component* component)
			: Operation(component)
		{
			jumpPress = false;
			yaw = 0;
			pitch = 0;
		}

		void Execute() override;
	};

	class CameraMovement : public Engine::Component
	{
		friend CameraMovementOperation;

		PxController* controller;
		Engine::Camera *camera;
		Engine::Camera *reflectionCamera;
		Engine::Camera *refractionCamera;
		Engine::SpotLight* spotLight;
		Engine::Entity *skybox;
		Player *player;
	public:
		explicit CameraMovement(Engine::SpotLight *spotLight, Engine::Camera *reflectionCamera, Engine::Camera *refractionCamera, Player *player, Engine::Entity *skybox);
		CameraMovement();

		void Init() override;
		void Wire() override;
		PxController *GetController() const;
	};
	extern const CameraMovement CameraMovementClass;
}
