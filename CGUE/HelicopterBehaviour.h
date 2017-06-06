#pragma once
#include "Component.h"
#include "Operation.h"
#include <cstdlib>
#include "SpotLight.h"
#include "Player.h"

namespace Engine {
	class HeightMapResource;
}

namespace Game {

	enum HelicopterMovementState
	{
		HMS_ONLY_ROTATE = 1,
		HMS_FOLLOW_PLAYER,
		HMS_FORWARD,
		HMS_LEFT_FORWARD,
		HMS_RIGHT_FORWARD
	};

	class HelicopterBehaviourOperation : public Engine::Operation
	{
		
	public:
		void ChangeDirection();

		explicit HelicopterBehaviourOperation(Engine::Component* component)
			: Operation(component)
		{
			ChangeDirection();
		}

		void Execute() override;
	};


	class HelicopterBehaviour : public Engine::Component
	{
		friend HelicopterBehaviourOperation;

		Engine::SpotLight *spotLight;
		bool broken;
		Engine::Transformation *mainRotor;
		Engine::Transformation *sideRotor;
		Engine::HeightMapResource *heightMap;

		HelicopterMovementState state;
		Player* player;
	public:
		explicit HelicopterBehaviour(Engine::Transformation *mainRotor, Engine::Transformation *sideRotor, bool broken, Engine::HeightMapResource *heightMap, Player *player);
		
		void Init() override;
		void Wire() override;
	};
}

