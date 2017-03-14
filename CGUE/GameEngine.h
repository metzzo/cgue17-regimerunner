#pragma once
#include "packages/sdl2.v140.2.0.4/build/native/include/SDL_video.h"
#include <queue>

using namespace std;

namespace Engine {
	class Entity;
	class GameState;
	class Camera;
	class Operation;

	typedef enum OPERATION_TYPE
	{
		UPDATE_OPERATION = 0,
		RENDER_PASS_OPERATION = 1,
		LIGHT_PASS_OPERATION = 2,
		DEPTH_PASS_OPERATION = 3,
	} OPERATION_TYPE;
	const int NUM_OPERATIONS = 10;

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

		vector<Operation*> operations[NUM_OPERATIONS];

		virtual void Init();
		virtual void DeInit();
		virtual void Render();
	public:
		GameEngine(int width, int height, string programName);
		virtual ~GameEngine();

		void Run();
		void RaiseEngineError(const string error);

		void AddOperation(Operation *operation);
		void ProcessQueue(OPERATION_TYPE type);
		vector<Operation*>* GetOperations(OPERATION_TYPE type);

		bool KeyDown(int keyCode);

		void SetMainCamera(Camera *mainCamera);
		Camera *GetMainCamera() const;
		string GetProgramName() const;
		Entity *GetRootEntity() const;
		int GetScreenWidth() const;
		int GetScreenHeight() const;
	};

}

