#pragma once

namespace Engine {
	class GameEngine;

	class GameState
	{
	public:
		GameState();
		virtual ~GameState();

		virtual void Update(GameEngine *engine) = 0;
	};
}
