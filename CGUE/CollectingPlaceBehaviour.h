#pragma once
#include "Component.h"
#include "Operation.h"

namespace Game {
	class Player;

	class CollectingPlaceOperation : public Engine::Operation
	{
	public:
		explicit CollectingPlaceOperation(Engine::Component* component)
			: Operation(component)
		{
		}

		void Execute() override;
		int GetPriority() override;
	};

	class CollectingPlaceBehaviour :
		public Engine::Component
	{
		friend CollectingPlaceOperation;

		Player* player;
	public:
		explicit CollectingPlaceBehaviour(Player *player);
		~CollectingPlaceBehaviour();

		void Init() override;
	};
}

