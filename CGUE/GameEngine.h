#pragma once
#include <vector>

using namespace std;

namespace Engine {
	class Entity;
	class GameState;
	class Camera;

	class GameEngine
	{
		string programName;
		bool cancelled;

	protected:
		Entity* rootEntity;
		Camera* mainCamera;
		int width;
		int height;

		void RaiseEngineError(const string error);

		virtual void Init() = 0;
		virtual void DeInit() = 0;

		virtual void Render() = 0;
	public:
		GameEngine(int width, int height, string programName);
		virtual ~GameEngine();

		void Run();

		void SetMainCamera(Camera *mainCamera);
		Camera *GetMainCamera() const;

		string GetProgramName() const;

		Entity *GetRootEntity() const;
		int GetScreenWidth() const;
		int GetScreenHeight() const;
	};

}

