#pragma once
#include "packages/sdl2.v140.2.0.4/build/native/include/SDL_video.h"
#include <queue>

using namespace std;

namespace Engine {
	class Entity;
	class GameState;
	class Camera;
	class Operation;

	typedef enum QUEUE_TYPE
	{
		QUEUE_UPDATE = 0,
		QUEUE_RENDER_PASS = 1,
		QUEUE_LIGHT_PASS = 2,
		QUEUE_DEPTH_PASS = 3,
	} QUEUE_TYPE;
	const int NUM_QUEUES = 10;

	class GameEngine
	{
		string programName;
		SDL_Window *mainwindow;
		SDL_GLContext maincontext;

		bool cancelled;
		Entity* rootEntity;
		Camera* mainCamera;
		int width;
		int height;
		bool keyStates[322];

		vector<Operation*> operations[NUM_QUEUES];

		virtual void Init();
		virtual void DeInit();
		virtual void Render();
	public:
		GameEngine(int width, int height, string programName);
		virtual ~GameEngine();

		void Run();
		void RaiseEngineError(const string error);

		void AddOperation(Operation *operation);
		void ProcessQueue(QUEUE_TYPE type);

		bool KeyDown(int keyCode);

		void SetMainCamera(Camera *mainCamera);
		Camera *GetMainCamera() const;
		string GetProgramName() const;
		Entity *GetRootEntity() const;
		int GetScreenWidth() const;
		int GetScreenHeight() const;
	};

}

