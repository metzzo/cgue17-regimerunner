#pragma once
#include "Component.h"
#include "packages/sdl2.v140.2.0.4/build/native/include/SDL_surface.h"
#include "glew/glew.h"
#include "BaseResource.h"

namespace Engine {
	class TextureResource : public BaseResource
	{
		SDL_Surface* sdlImage;
		GLuint textureId;
	public:
		explicit TextureResource(string filename);
		~TextureResource();

		void Load() override;
		GLuint GetTextureId() const;

		int GetWidth() const;
		int GetHeight() const;
		void* GetPixels() const;
		int GetMode() const;
		int GetBytesPerPixel() const;
	};
}
