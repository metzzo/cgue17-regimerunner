#include "GameEngine.h"
#include <iostream>
#include "GameState.h"

namespace Engine {
	GameEngine::GameEngine(Vector2i screenSize, const string programName, GameState *gameState)
	{
		this->screenSize = screenSize;
		this->programName = programName;
		this->cancelled = false;
		this->currentGameState = gameState;
	}


	GameEngine::~GameEngine()
	{
		delete this->currentGameState;
	}

	void GameEngine::Run()
	{
		this->Init();
		while(!cancelled)
		{
			if (this->currentGameState != nullptr) {
				this->currentGameState->Update(this);
			}

			this->Render();
		}
		this->DeInit();
	}

	Vector2i GameEngine::GetScreenSize() const
	{
		return this->screenSize;
	}

	string GameEngine::GetProgramName() const
	{
		return this->programName;
	}

	void GameEngine::RaiseEngineError(const string error)
	{
		cout << "Engine Error: " << error << endl;
		this->DeInit();
		exit(1);
	}
}
