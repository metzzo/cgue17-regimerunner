#include "GameEngine.h"
#include <iostream>
#include "Entity.h"
#include "Camera.h"
#include <SDL.h>
#include "glew/glew.h"
#include <string>
#include <SDL_image.h>
#include <SDL_audio.h>
#include "Pass.h"
#include "RenderPass.h"
#include "DepthPass.h"
#include "RigidBody.h"
#include "glm/glm.hpp"
#include "Component.h"
#include "glm/gtx/transform.hpp"
#include "Timer.h";
#include <fstream>

using namespace std;

namespace Engine {
	void checkSDLError(int line = -1) {
#ifndef NDEBUG
		auto error = SDL_GetError();
		if (*error != '\0')
		{
			printf("SDL Error: %s\n", error);
			if (line != -1)
				printf(" + line: %i\n", line);
			SDL_ClearError();
		}
#endif
	}

	GLenum glCheckError_(const char *file, int line)
	{
		GLenum errorCode;
		while ((errorCode = glGetError()) != GL_NO_ERROR)
		{
			std::string error;
			switch (errorCode)
			{
			case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
			case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
			case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
			case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
			case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
			case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
			default:							   error = "Unknown"; break;
			}
			std::cout << error << " | " << file << " (" << line << ")" << std::endl;
			getchar();
			exit(1);
		}
		return errorCode;
	}

	void RaiseEngineError(const string error)
	{
		cout << "Engine Error: " << error << endl;
		getchar();
		exit(1);
	}

	GameEngine::GameEngine(int width, int height, bool fullscreen, const string programName)
	{

		this->programName = programName;
		this->cancelled = false;
		this->width = width;
		this->height = height;
		this->fullscreen = fullscreen;
		this->rootEntity = new Entity(this);
		this->mainCamera = nullptr;
		this->utilityCamera = nullptr;

		memset(this->keyStates, 0, sizeof this->keyStates);
		memset(this->keyStatesOld, 0, sizeof this->keyStates);

		this->foundation = nullptr;
		this->dispatcher = nullptr;
		this->scene = nullptr;
		this->physics = nullptr;

		this->showFPSEnabled = false;
		this->wireFrameEnabled = false;
		this->textureSamplingQuality = true;
		this->waterEnabled = true;
		this->callHelicopter = false;
		this->frustumCullingEnabled = true;
		this->switchMipMappingQuality = false;
		this->switchTextureSamplingQuality = false;

		this->renderPass = new RenderPass(this);
		this->updatePass = new Pass(this);
		this->depthPass = new DepthPass(this);
		this->cameraPass = new Pass(this);

		this->mouseXRel = 0;
		this->mouseYRel = 0;

		this->started = false;
	}


	GameEngine::~GameEngine()
	{
		delete updatePass;
		delete renderPass;
		delete depthPass;

		delete this->rootEntity;

		// TODO: clear other stuff too
	}

	void GameEngine::Run()
	{
		this->Init();

		this->rootEntity->Wire();

		this->rootEntity->Init();
		
		this->started = true;
		this->lastTime = SDL_GetTicks();
		

		while(!cancelled && !KeyDown(SDL_SCANCODE_ESCAPE))
		{
			this->mouseXRel = 0;
			this->mouseYRel = 0;
			auto newTime = SDL_GetTicks();
			this->deltaTime = newTime - this->lastTime;
			this->lastTime = newTime;

			if (this->showFPSEnabled) {
				if (newTime > 1000) {
					//int fps = 1 / (1 / deltaTime);
					double frames = 1000 / deltaTime;
					cout << "FPS: " << frames << endl;
				}
			}

			SDL_Event e;
			while (SDL_PollEvent(&e) != 0)
			{
				//User requests quit
				switch (e.type)
				{
				case SDL_QUIT:
					cancelled = true;
					break;
				case SDL_KEYDOWN:
					if (e.key.repeat == 0) {
						this->keyStatesOld[e.key.keysym.scancode] = true;
						this->keyStates[e.key.keysym.scancode] = true;
					}
					break;
				case SDL_KEYUP:
					if (e.key.repeat == 0) {
						this->keyStates[e.key.keysym.scancode] = false;
						this->DoSwitchTextureSamplingQuality();
						this->DoSwitchMipMappingQuality();
					}
					break;
				case SDL_MOUSEMOTION:
					this->mouseXRel = e.motion.xrel;
					this->mouseYRel = e.motion.yrel;
					break;
				default:
					break;
				}
			}

			if ( this->KeyDown(SDL_SCANCODE_F1)) {
				cout << "Help? " << "Here is some help (F1) " << endl;
				cout << "WASD: " << "Walk around" << endl;
				cout << "Space: " << "Jump!" << endl;
				cout << "E: " << "Chop a palm" << endl;
				cout << "F1: " << "Call for help! " << endl;
				cout << "F2: " << "Show FPS" << endl;
				cout << "F3: " << "Wireframe on/of" << endl;
				cout << "F4: " << "Texture Sampling quality " << endl;
				cout << "F5: " << "Mip mapping quality" << endl;
				cout << "F6: " << "Enable/Disable water" << endl;
				cout << "F7: " << "Call/Uncall Helicopter" << endl;
				cout << "F8: " << "Enable/Disable view frustum culling " << endl;
				cout << "F9: " << "Enable/Disable blending" << endl;
				this->keyStatesOld[SDL_SCANCODE_F1] = false;
			}

			if (this->KeyDownLastFrame(SDL_SCANCODE_F2)) {
				if (!this->showFPSEnabled) {
					this->showFPSEnabled = true;
					this->keyStatesOld[SDL_SCANCODE_F2] = false;
				}
				else {
					this->showFPSEnabled = false;
					this->keyStatesOld[SDL_SCANCODE_F2] = false;
				}
			}

			if (this->KeyDownLastFrame(SDL_SCANCODE_F3)) {

				if (!this->wireFrameEnabled) {
					cout << "F3: " << "Enabling Wireframe" << endl;
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					this->wireFrameEnabled = true;
					this->keyStatesOld[SDL_SCANCODE_F3] = false;
				}
				else {
					cout << "F3: " << "Disabling Wireframe" << endl;
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					this->wireFrameEnabled = false;
					this->keyStatesOld[SDL_SCANCODE_F3] = false;
				}
			}

			if (this->KeyDownLastFrame(SDL_SCANCODE_F4)) {
				if (!this->textureSamplingQuality) {
					cout << "F4: " << "Setting Texture Sampling Quality to GL_LINEAR" << endl;
					this->textureSamplingQuality = true;
					this->keyStatesOld[SDL_SCANCODE_F4] = false;
					this->DoSwitchTextureSamplingQuality();

				}
				else {
					cout << "F4: " << "Setting Texture Sampling Quality to GL_NEAREST" << endl;
					this->textureSamplingQuality = false;
					this->keyStatesOld[SDL_SCANCODE_F4] = false;
					this->DoSwitchTextureSamplingQuality();
				}
			}

			if (this->KeyDownLastFrame(SDL_SCANCODE_F5)) {
				this->mipMappingQuality += 1;
				if (this->mipMappingQuality == 5) {
					this->mipMappingQuality = 0;
				}
				cout << "F5: Setting Mip Map Level to: " << this->mipMappingQuality << endl;
				this->keyStatesOld[SDL_SCANCODE_F5] = false;
				this->DoSwitchMipMappingQuality();
			}

			if (this->KeyDownLastFrame(SDL_SCANCODE_F6)) {
				if (!this->waterEnabled) {
					cout << "F6: " << "Enabling Water" << endl;
					this->waterEnabled = true;
					this->keyStatesOld[SDL_SCANCODE_F6] = false;
				}
				else {
					cout << "F6: " << "Disabling Water" << endl;
					this->waterEnabled = false;
					this->keyStatesOld[SDL_SCANCODE_F6] = false;
				}
			}

			if (this->KeyDownLastFrame(SDL_SCANCODE_F7)) {
				if (!this->callHelicopter) {
					cout << "F7: " << "Call Helicopter" << endl;
					this->callHelicopter = true;
					this->keyStatesOld[SDL_SCANCODE_F7] = false;
				}
				else {
					cout << "F7: " << "Uncall Helicopter" << endl;
					this->callHelicopter = false;
					this->keyStatesOld[SDL_SCANCODE_F7] = false;
				}
			}

			if (this->KeyDownLastFrame(SDL_SCANCODE_F8)) {
				if (!this->frustumCullingEnabled) {
					cout << "F8: " << "Enabling Frustum Culling" << endl;
					this->frustumCullingEnabled = true;
					this->keyStatesOld[SDL_SCANCODE_F8] = false;
				}
				else {
					cout << "F8: " << "Disabling Frustum Culling" << endl;
					this->frustumCullingEnabled = false;
					this->keyStatesOld[SDL_SCANCODE_F8] = false;
				}
			}

			if (this->KeyDownLastFrame(SDL_SCANCODE_F9)) {
				if (!this->blendingEnabled) {
					cout << "F9: " << "Enabling Blending" << endl;
					this->blendingEnabled = true;
					this->keyStatesOld[SDL_SCANCODE_F9] = false;
				}
				else {
					cout << "F9: " << "Disabling Blending" << endl;
					this->blendingEnabled = false;
					this->keyStatesOld[SDL_SCANCODE_F9] = false;
				}
			}


			if (initComponents.size() > 0)
			{
				for (auto &component : initComponents)
				{
					component->Wire();
				}
				
				for (auto &component : initComponents)
				{
					component->Init();
				}
				initComponents.clear();
			}

			UpdatePhysics();

			GetUpdatePass()->DoPass();

			this->Render();


			this->keyStatesOld[SDL_SCANCODE_E] = false; // dirty me is dirty, this should definitely NOT be done like that

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

	float GameEngine::GetDeltaTime() const
	{
		return this->deltaTime;
	}

	PxCooking* GameEngine::GetCooking() const
	{
		return this->cooking;
	}

	PxPhysics* GameEngine::GetPhysics() const
	{
		return this->physics;
	}

	PxScene* GameEngine::GetPhysicsScene() const
	{
		return scene;
	}

	PxControllerManager* GameEngine::GetControllerManager() const
	{
		return manager;
	}

	void GameEngine::AddInitComponent(Component* component)
	{
		this->initComponents.push_back(component);
	}

	bool GameEngine::GetTextureSamplingQuality() const
	{
		return this->textureSamplingQuality;
	}

	int GameEngine::GetMipMappingQuality() const
	{
		return this->mipMappingQuality;
	}

	void GameEngine::SetMipMappingQuality(int x)
	{
		this->mipMappingQuality = x;
	}

	bool GameEngine::IsWaterEnabled() const
	{
		return this->waterEnabled;
	}

	bool GameEngine::IsHelicopterCalled() const
	{
		return this->callHelicopter;
	}

	bool GameEngine::IsCullingEnabled() const
	{
		return this->frustumCullingEnabled;
	}

	bool GameEngine::IsBlendingEnabled() const
	{
		return this->blendingEnabled;
	}

	bool GameEngine::MipMappingSwitched()
	{
		return this->switchMipMappingQuality;
	}

	bool GameEngine::TextureSamplingSwitched()
	{
		return this->switchTextureSamplingQuality;
	}

	void GameEngine::DoSwitchMipMappingQuality()
	{
		this->switchMipMappingQuality = !this->switchMipMappingQuality;
	}

	void GameEngine::DoSwitchTextureSamplingQuality()
	{
		this->switchTextureSamplingQuality = !this->switchTextureSamplingQuality;
	}


	bool GameEngine::IsStarted() const
	{
		return started;
	}

	bool GameEngine::KeyDown(int keyCode)
	{
		return this->keyStates[keyCode];
	}

	bool GameEngine::KeyDownLastFrame(int keyCode)
	{
		return this->keyStatesOld[keyCode];
	}

	RenderPass* GameEngine::GetRenderPass() const
	{
		return renderPass;
	}

	DepthPass* GameEngine::GetDepthPass() const
	{
		return depthPass;
	}

	Pass* GameEngine::GetUpdatePass() const
	{
		return updatePass;
	}
	Pass* GameEngine::GetCameraPass() const
	{
		return cameraPass;
	}

	int GameEngine::GetMouseXRel() const
	{
		return mouseXRel;
	}

	int GameEngine::GetMouseYRel() const
	{
		return mouseYRel;
	}


	void GameEngine::Init()
	{
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			RaiseEngineError("Unable to initialize SDL: " + string(SDL_GetError()));
		}

		// load support for the JPG and PNG image formats
		auto flags = IMG_INIT_JPG | IMG_INIT_PNG;
		auto initted = IMG_Init(flags);
		if ((initted&flags) != flags) {
			RaiseEngineError("IMG_Init: Failed to init required jpg and png support: %s" + string(IMG_GetError()));
		}

		// set opengl versions
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

		// set certain attributes
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		//SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		//SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		auto programName = this->GetProgramName().c_str();

		this->mainwindow = SDL_CreateWindow(
			programName,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			width,
			height,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
		);

		if (!this->mainwindow)
		{
			RaiseEngineError("Unable to create window: " + string(SDL_GetError()));
		}
		checkSDLError(__LINE__);

		SDL_SetWindowGrab(this->mainwindow, SDL_TRUE);
		SDL_ShowCursor(SDL_DISABLE);

		if (this->fullscreen == true) {
			SDL_SetWindowFullscreen(this->mainwindow, SDL_WINDOW_FULLSCREEN);
		}

		this->maincontext = SDL_GL_CreateContext(this->mainwindow);
		checkSDLError(__LINE__);

		glewExperimental = GL_TRUE;
		auto error = glewInit();
		if (GLEW_OK != error)
		{
			RaiseEngineError("Glew init error: " + to_string(error));
		}
		SDL_GL_SetSwapInterval(0);

		cout << "Using OpenGL Version: " << glGetString(GL_VERSION) << endl;

		glClearColor(0.0, 0.0, 0.0, 1.0);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		// init physics
		this->physicsAccumulator = 0.0f;
		this->physicsStepSize = 1.0f / 30.0f;

		this->foundation = PxCreateFoundation(PX_FOUNDATION_VERSION, this->allocator, this->errorCallback);

		auto pvd = PxCreatePvd(*foundation);
		auto transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
		pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

		this->physics = PxCreatePhysics(PX_PHYSICS_VERSION, *this->foundation, PxTolerancesScale(), true, pvd);
		this->cooking = PxCreateCooking(PX_PHYSICS_VERSION, *this->foundation, PxCookingParams(PxTolerancesScale()));
		PxSceneDesc sceneDesc(this->physics->getTolerancesScale());
		sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
		this->dispatcher = PxDefaultCpuDispatcherCreate(2);

		sceneDesc.cpuDispatcher = this->dispatcher;
		sceneDesc.filterShader = PxDefaultSimulationFilterShader;
		this->scene = this->physics->createScene(sceneDesc);
		this->scene->setFlag(PxSceneFlag::eENABLE_ACTIVE_ACTORS, true);
		this->manager = PxCreateControllerManager(*this->scene);

		// setup physx debugger

		this->renderPass->Init();
		this->updatePass->Init();
		this->depthPass->Init();
		this->cameraPass->Init();
	}

	void GameEngine::DeInit()
	{
		scene->release();
		dispatcher->release();
		physics->release();
		cooking->release();
		foundation->release();

		SDL_GL_DeleteContext(this->maincontext);
		SDL_Quit();
		IMG_Quit();
	}

	void GameEngine::Render()
	{
		GetCameraPass()->DoPass();
		
		SDL_GL_SwapWindow(this->mainwindow);
	}

	void GameEngine::UpdatePhysics() const
	{
		if (deltaTime == 0)
		{
			return;
		}
		scene->simulate(deltaTime*physicsStepSize*0.1f);
		scene->fetchResults(true);

		PxU32 nbActiveActors;
		auto activeActors = scene->getActiveActors(nbActiveActors);

		// update each render object with the new transform
		for (PxU32 i = 0; i < nbActiveActors; ++i)
		{
			if (activeActors[i]->userData == nullptr)
			{
				continue;
			}

			auto rigidBody = static_cast<RigidBody*>(activeActors[i]->userData);
			auto actor = rigidBody->GetActor();
			rigidBody->GetTransformation()->UpdatePhysicsMatrix(actor->getGlobalPose());
		}
	}
}
