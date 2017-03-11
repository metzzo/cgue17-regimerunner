#pragma once
#include "Component.h"
#include "packages/sdl2.v140.2.0.4/build/native/include/SDL_surface.h"

namespace Engine {
	class Texture :
		public Component
	{
		string filename;
		SDL_Surface* sdlImage;
	public:
		Texture();
		explicit Texture(string filename);
		~Texture();
	};
}
