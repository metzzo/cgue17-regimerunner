#pragma once
#include "Component.h"
#include "packages/sdl2.v140.2.0.4/build/native/include/SDL_surface.h"
#include "glew/glew.h"

namespace Engine {
	class Texture
	{
		string filename;
		SDL_Surface* sdlImage;
		GLuint textureId;
	public:
		Texture();
		explicit Texture(string filename);
		~Texture();

		void Init();
		GLuint GetTextureId() const;
		string& GetFileName();
	};
}
