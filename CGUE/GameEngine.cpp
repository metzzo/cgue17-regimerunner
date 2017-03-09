#include "GameEngine.h"
#include <iostream>
#include "Entity.h"
#include "Camera.h"

namespace Engine {
	GameEngine::GameEngine(int width, int height, const string programName)
	{
		this->programName = programName;
		this->cancelled = false;
		this->width = width;
		this->height = height;

		this->rootEntity = new Entity(this);
		this->mainCamera = nullptr;
	}


	GameEngine::~GameEngine()
	{

	}

	void GameEngine::Run()
	{
		this->Init();

		this->rootEntity->Wire();

		this->rootEntity->Init();

		this->rootEntity->GetTransformation()->SetProjectionViewMatrix(this->mainCamera->GetProjectionViewMatrix());

		while(!cancelled)
		{
			this->rootEntity->Update();

			this->Render();
		}
		this->DeInit();
	}

	void GameEngine::SetMainCamera(Camera* mainCamera)
	{
		this->mainCamera = mainCamera;

	}

	Camera* GameEngine::GetMainCamera() const
	{
		return this->mainCamera;
	}

	string GameEngine::GetProgramName() const
	{
		return this->programName;
	}

	Entity* GameEngine::GetRootEntity() const
	{
		return this->rootEntity;
	}

	int GameEngine::GetScreenWidth() const
	{
		return this->width;
	}

	int GameEngine::GetScreenHeight() const
	{
		return this->height;
	}

	void GameEngine::RaiseEngineError(const string error)
	{
		//cout << "Engine Error: " << error << endl;
		this->DeInit();
		exit(1);
	}
}
