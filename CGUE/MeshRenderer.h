#pragma once
#include "Renderer.h"
namespace Engine {
	class MeshRenderer :
		public Renderer
	{
	public:
		MeshRenderer();
		~MeshRenderer();

		void Render(GameEngine *gameEngine) override;
	};
}

