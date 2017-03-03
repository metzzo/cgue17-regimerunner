#include "GameEngine.h"
#include <iostream>
#include "GameState.h"
#include "Entity.h"

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
		if (this->currentGameState != nullptr) {
			this->currentGameState->Init(this);
		}

		for (auto &entity : this->entities)
		{
			entity->Wire();
		}

		for (auto &entity : this->entities)
		{
			entity->Init(this);
		}

		while(!cancelled)
		{
			for (auto &entity : this->entities)
			{
				entity->Update(this);
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

	Entity* GameEngine::CreateEntity()
	{
		auto entity = new Entity();
		this->entities.push_back(entity);
		return entity;
	}

	void GameEngine::RaiseEngineError(const string error)
	{
		cout << "Engine Error: " << error << endl;
		this->DeInit();
		exit(1);
	}
}
