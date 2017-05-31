#pragma once
#include "packages/sdl2.v140.2.0.4/build/native/include/SDL_video.h"
#include "glew/glew.h"
#include <string>
#include <vector>
#include <ctype.h>
#include "PxPhysicsAPI.h"
#include "Component.h"

using namespace physx;
using namespace std;

namespace Engine {
	class Entity;
	class GameState;
	class Camera;
	class Pass;
	class RenderPass;
	class DepthPass;
	class Component;
	class WaterPass;

	void RaiseEngineError(const string error);
	
	GLenum glCheckError_(const char *file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__);
#define DEBUG_OGL(X) X; glCheckError_(__FILE__, __LINE__); 

	class GameEngine
	{
	protected:
		string programName;
		SDL_Window *mainwindow;
		SDL_GLContext maincontext;

		int lastTime;
		int deltaTime;

		bool cancelled;
		Entity* rootEntity;
		Camera* mainCamera;
		int width;
		int height;
		int mouseXRel;
		int mouseYRel;
		bool keyStates[322];

		vector<Component*> initComponents;

		Pass *updatePass;
		Pass *cameraPass;
		DepthPass *depthPass;
		RenderPass *renderPass;
		WaterPass *waterPass;

		PxDefaultAllocator		allocator;
		PxDefaultErrorCallback	errorCallback;

		PxFoundation*			foundation;
		PxPhysics*				physics;
		PxCooking*				cooking;

		PxDefaultCpuDispatcher*	dispatcher;
		PxScene*				scene;

		float physicsAccumulator;
		float physicsStepSize;
		PxControllerManager* manager;
		bool started;

		virtual void Init();
		virtual void DeInit();
		virtual void Render();

		void UpdatePhysics();
	public:
		GameEngine(int width, int height, string programName);
		virtual ~GameEngine();

		void Run();

		bool IsStarted() const;

		bool KeyDown(int keyCode);

		RenderPass *GetRenderPass() const;
		DepthPass *GetDepthPass() const;
		WaterPass *GetWaterPass() const;
		Pass *GetUpdatePass() const;
		Pass *GetCameraPass() const;

		int GetMouseXRel() const;
		int GetMouseYRel() const;

		void SetMainCamera(Camera *mainCamera);
		Camera *GetMainCamera() const;
		string GetProgramName() const;
		Entity *GetRootEntity() const;
		int GetScreenWidth() const;
		int GetScreenHeight() const;
		float GetDeltaTime() const;
	
		PxCooking *GetCooking() const;
		PxPhysics *GetPhysics() const;
		PxScene *GetPhysicsScene() const;
		PxControllerManager *GetControllerManager() const;
		void AddInitComponent(Component* component);
	};

}

