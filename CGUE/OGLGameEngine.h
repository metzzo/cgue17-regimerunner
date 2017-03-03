#pragma once
#include "GameEngine.h"
#include <SDL.h>

namespace Engine {
	class OGLGameEngine : public GameEngine
	{
		SDL_Window *mainwindow;
		SDL_GLContext maincontext;

	public:
		void Init() override;
		void DeInit() override;
		void Render() override;

		using GameEngine::GameEngine;
	};
}

