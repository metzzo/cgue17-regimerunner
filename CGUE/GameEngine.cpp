#include "GameEngine.h"
#include <iostream>
#include "Entity.h"
#include "Camera.h"
#include <SDL.h>
#include "glew/glew.h"
#include <string>
#include <SDL_image.h>
#include "Pass.h"
#include "RenderPass.h"
#include "DepthPass.h"
#include "RigidBody.h"
#include "glm/glm.hpp"
#include "Component.h"
#include "glm/gtx/transform.hpp"


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

	GameEngine::GameEngine(int width, int height, const string programName)
	{
		this->programName = programName;
		this->cancelled = false;
		this->width = width;
		this->height = height;

		this->rootEntity = new Entity(this);
		this->mainCamera = nullptr;

		memset(this->keyStates, 0, sizeof this->keyStates);

		this->foundation = nullptr;
		this->dispatcher = nullptr;
		this->scene = nullptr;
		this->physics = nullptr;

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
						this->keyStates[e.key.keysym.scancode] = true;
					}
					break;
				case SDL_KEYUP:
					if (e.key.repeat == 0) {
						this->keyStates[e.key.keysym.scancode] = false;
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

	void GameEngine::AddLight(SpotLight* spotLight)
	{
		this->lights.push_back(spotLight);
	}

	vector<SpotLight*>& GameEngine::GetLights()
	{
		return this->lights;
	}

	bool GameEngine::IsStarted() const
	{
		return started;
	}

	bool GameEngine::KeyDown(int keyCode)
	{
		return this->keyStates[keyCode];
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

		this->maincontext = SDL_GL_CreateContext(this->mainwindow);
		checkSDLError(__LINE__);

		glewExperimental = GL_TRUE;
		auto error = glewInit();
		if (GLEW_OK != error)
		{
			RaiseEngineError("Glew init error: " + to_string(error));
		}

		SDL_GL_SetSwapInterval(1);

		cout << "Using OpenGL Version: " << glGetString(GL_VERSION) << endl;

		glClearColor(0.0, 0.0, 0.0, 1.0);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		// init physics
		this->physicsAccumulator = 0.0f;
		this->physicsStepSize = 1.0f / 60.0f;

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

	void GameEngine::UpdatePhysics()
	{
		if (deltaTime == 0)
		{
			return;
		}
		scene->simulate(deltaTime*physicsStepSize);
		scene->fetchResults(true);

		PxU32 nbActiveActors;
		PxActor** activeActors = scene->getActiveActors(nbActiveActors);

		// update each render object with the new transform
		for (PxU32 i = 0; i < nbActiveActors; ++i)
		{
			if (activeActors[i]->userData == nullptr)
			{
				continue;
			}

			RigidBody* rigidBody = static_cast<RigidBody*>(activeActors[i]->userData);
			auto shape = rigidBody->GetShape();
			auto actor = rigidBody->GetActor();
			rigidBody->GetTransformation()->UpdatePhysicsMatrix(PxShapeExt::getGlobalPose(*shape, *actor));
		}
	}
}
