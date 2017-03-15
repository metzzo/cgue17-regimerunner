#pragma once
#include "packages/sdl2.v140.2.0.4/build/native/include/SDL_video.h"
#include <queue>
#include "glew/glew.h"

using namespace std;

namespace Engine {
	class Entity;
	class GameState;
	class Camera;
	class Operation;
	
	GLenum glCheckError_(const char *file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__);
#define DEBUG_OGL(X) X; glCheckError_(__FILE__, __LINE__); 
	typedef enum OPERATION_TYPE
	{
		UNDEFINED_OPERATION = -1,
		UPDATE_OPERATION = 0,
		CAMERA_PASS_OPERATION = 1,
		DEPTH_PASS_OPERATION = 2,
		RENDER_PASS_OPERATION = 3,
	} OPERATION_TYPE;
	const int NUM_OPERATIONS = 10;

	class GameEngine
	{
	protected:
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


		void SortPriorities();
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

