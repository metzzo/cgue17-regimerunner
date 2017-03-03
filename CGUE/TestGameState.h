#pragma once
#include "GameState.h"

using namespace Engine;

namespace Game {
	class TestGameState : public GameState
	{
	public:
		using GameState::GameState;
		void Update(GameEngine *engine) override;
	};
}
