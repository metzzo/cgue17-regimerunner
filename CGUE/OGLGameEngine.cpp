#include "OGLGameEngine.h"
#include <SDL.h>
#include "glew/glew.h"
#include "Entity.h"
#include <iostream>

namespace Engine {
	void checkSDLError(int line = -1)
	{
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

	void OGLGameEngine::Init()
	{
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			this->RaiseEngineError("Unable to initialize SDL: " + string(SDL_GetError()));
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
		auto screenSize = this->GetScreenSize();

		this->mainwindow = SDL_CreateWindow(
			programName, 
			SDL_WINDOWPOS_CENTERED, 
			SDL_WINDOWPOS_CENTERED,
			screenSize.x(),
			screenSize.y(), 
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
	}

	void OGLGameEngine::DeInit()
	{
		SDL_GL_DeleteContext(this->maincontext);
		SDL_Quit();
	}

	void OGLGameEngine::Render()
	{
		glClear(GL_COLOR_BUFFER_BIT);

		for (auto &entity : this->entities)
		{
			entity->Render();
		}

		SDL_GL_SwapWindow(this->mainwindow);
	}
}
