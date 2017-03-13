#pragma once
#include <vector>
#include "packages/sdl2.v140.2.0.4/build/native/include/SDL_video.h"

using namespace std;

namespace Engine {
	class Entity;
	class GameState;
	class Camera;

	class GameEngine
	{
		string programName;
		SDL_Window *mainwindow;
		SDL_GLContext maincontext;
	protected:
		bool cancelled;
		Entity* rootEntity;
		Camera* mainCamera;
		int width;
		int height;
		bool keyStates[322];

		virtual void Init();
		virtual void DeInit();
		virtual void Render();
	public:
		GameEngine(int width, int height, string programName);
		virtual ~GameEngine();

		void Run();
		void RaiseEngineError(const string error);

		bool KeyDown(int keyCode);

		void SetMainCamera(Camera *mainCamera);
		Camera *GetMainCamera() const;
		string GetProgramName() const;
		Entity *GetRootEntity() const;
		int GetScreenWidth() const;
		int GetScreenHeight() const;
	};

}

