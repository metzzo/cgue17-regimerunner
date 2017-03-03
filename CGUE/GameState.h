#pragma once

namespace Engine {
	class GameEngine;

	class GameState
	{
	public:
		GameState();
		virtual ~GameState();

		virtual void Init(GameEngine *engine) = 0;
	};
}
