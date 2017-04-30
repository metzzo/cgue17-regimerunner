#pragma once
#include "Operation.h"

namespace Game {

	class CameraMovementMouseOperation : public Engine::Operation
	{
	public:
		explicit CameraMovementMouseOperation(Engine::Component* component)
			: Operation(component)
		{
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
		void Init() override;
	};
}
