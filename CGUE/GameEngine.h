#pragma once
#include "packages/sdl2.v140.2.0.4/build/native/include/SDL_video.h"
#include "glew/glew.h"
#include <string>
#include <vector>

using namespace std;

namespace Engine {
	class Entity;
	class GameState;
	class Camera;
	class SpotLight;
	class Pass;
	class RenderPass;
	class DepthPass;

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

		bool cancelled;
		Entity* rootEntity;
		Camera* mainCamera;
		int width;
		int height;
		float mousexold;
		float mouseyold;
		float mousexoffset;
		float mouseyoffset;
		bool keyStates[322];

		vector<SpotLight*> lights;

		Pass *updatePass;
		Pass *cameraPass;
		DepthPass *depthPass;
		RenderPass *renderPass;

		virtual void Init();
		virtual void DeInit();
		virtual void Render();
	public:
		GameEngine(int width, int height, string programName);
		virtual ~GameEngine();

		void Run();

		void AddLight(SpotLight* spotLight);
		// TODO: add RemoveLight
		vector<SpotLight*>& GetLights();

		bool KeyDown(int keyCode);

		RenderPass *GetRenderPass() const;
		DepthPass *GetDepthPass() const;
		Pass *GetUpdatePass() const;
		Pass* GetCameraPass() const;

		void SetMainCamera(Camera *mainCamera);
		Camera *GetMainCamera() const;
		string GetProgramName() const;
		Entity *GetRootEntity() const;
		int GetScreenWidth() const;
		int GetScreenHeight() const;
		float GetMouseXOffset();
		float GetMouseYOffset();
	};

}

