#include "GameEngine.h"
#include <iostream>
#include "Entity.h"
#include "Camera.h"
#include <SDL.h>
#include "glew/glew.h"
#include <string>
#include <SDL_image.h>
#include "Operation.h"
#include "Shader.h"

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


	GameEngine::GameEngine(int width, int height, const string programName)
	{
		this->programName = programName;
		this->cancelled = false;
		this->width = width;
		this->height = height;

		this->rootEntity = new Entity(this);
		this->mainCamera = nullptr;

		memset(this->keyStates, 0, sizeof this->keyStates);
	}


	GameEngine::~GameEngine()
	{
		for (auto i = 0; i < NUM_OPERATIONS; i++)
		{
			for (auto &operation : this->operations[i])
			{
				delete operation;
			}
		}
	}

	void GameEngine::Run()
	{
		this->Init();

		this->rootEntity->Wire();

		this->rootEntity->Init();

		this->SortPriorities();
		
		while(!cancelled)
		{
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
				default:
					break;
				}
			}

			ProcessQueue(UPDATE_OPERATION);

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
		cout << "Engine Error: " << error << endl;
		getchar();
		this->DeInit();
		exit(1);
	}

	void GameEngine::AddLight(SpotLight* spotLight)
	{
		this->lights.push_back(spotLight);
	}

	vector<SpotLight*>& GameEngine::GetLights()
	{
		return this->lights;
	}

	bool GameEngine::KeyDown(int keyCode)
	{
		return this->keyStates[keyCode];
	}


	void GameEngine::Init()
	{
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			this->RaiseEngineError("Unable to initialize SDL: " + string(SDL_GetError()));
		}

		// load support for the JPG and PNG image formats
		auto flags = IMG_INIT_JPG | IMG_INIT_PNG;
		auto initted = IMG_Init(flags);
		if ((initted&flags) != flags) {
			this->RaiseEngineError("IMG_Init: Failed to init required jpg and png support: %s" + string(IMG_GetError()));
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
			this->RaiseEngineError("Unable to create window: " + string(SDL_GetError()));
		}
		checkSDLError(__LINE__);

		this->maincontext = SDL_GL_CreateContext(this->mainwindow);
		checkSDLError(__LINE__);

		glewExperimental = GL_TRUE;
		auto error = glewInit();
		if (GLEW_OK != error)
		{
			this->RaiseEngineError("Glew init error: " + to_string(error));
		}

		SDL_GL_SetSwapInterval(1);

		cout << "Using OpenGL Version: " << glGetString(GL_VERSION) << endl;

		glClearColor(0.0, 0.0, 1.0, 1.0);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		this->defaultShader = new Shader("materials/default_material.vert", "materials/default_material.frag");
	}

	void GameEngine::DeInit()
	{
		SDL_GL_DeleteContext(this->maincontext);
		SDL_Quit();
		IMG_Quit();
	}

	void GameEngine::Render()
	{
		ProcessQueue(CAMERA_PASS_OPERATION);
		
		SDL_GL_SwapWindow(this->mainwindow);
	}

	bool PrioritySortFunc(Operation *a, Operation *b)
	{
		return a->GetPriority() < b->GetPriority();
	}

	void GameEngine::SortPriorities()
	{
		for (auto i = 0; i < NUM_OPERATIONS; i++)
		{
			auto operations = &this->operations[i];
			sort(operations->begin(), operations->end(), PrioritySortFunc);
		}
	}
}
