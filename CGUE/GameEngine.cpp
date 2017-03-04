#include "GameEngine.h"
#include <iostream>
#include "Entity.h"

namespace Engine {
	GameEngine::GameEngine(Vector2i screenSize, const string programName)
	{
		this->screenSize = screenSize;
		this->programName = programName;
		this->cancelled = false;
	}


	GameEngine::~GameEngine()
	{

	}

	void GameEngine::Run()
	{
		this->Init();

		for (auto &entity : this->entities)
		{
			entity->Wire();
		}

		for (auto &entity : this->entities)
		{
			entity->Init();
		}

		while(!cancelled)
		{
			for (auto &entity : this->entities)
			{
				entity->Update();
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
		auto entity = new Entity(this);
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
