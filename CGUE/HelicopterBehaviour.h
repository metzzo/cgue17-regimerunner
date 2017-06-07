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
		HMS_FOLLOW_PLAYER=0,
		HMS_FORWARD,
		HMS_LEFT_FORWARD,
		HMS_RIGHT_FORWARD,
		HMS_GOTO_CENTER
	};

	class HelicopterBehaviourOperation : public Engine::Operation
	{
		
	public:
		void ChangeDirection() const;

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

		int state;
		Player* player;
		int start;
		int duration;
	public:
		explicit HelicopterBehaviour(Engine::Transformation *mainRotor, Engine::Transformation *sideRotor, bool broken, Engine::HeightMapResource *heightMap, Player *player);
		
		void Init() override;
		void Wire() override;
	};
}

