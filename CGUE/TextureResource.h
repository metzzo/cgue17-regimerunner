#pragma once
#include "Component.h"
#include "packages/sdl2.v140.2.0.4/build/native/include/SDL_surface.h"
#include "glew/glew.h"
#include "BaseResource.h"
#include "TextureRenderable.h"

using namespace std;

namespace Engine {
	class TextureResource : public BaseResource, public TextureRenderable
	{
		SDL_Surface* sdlImage;
		GLuint textureId;
	public:
		explicit TextureResource(string filename);
		explicit TextureResource(GLubyte *pixels, int width, int height);
		~TextureResource();

		void Load() override;

		GLuint GetTextureId() override;
		int GetWidth() override;
		int GetHeight() override;
		void Prepare() override;

		void* GetPixels() const;
		int GetBytesPerPixel() const;
		void Save(string filename) const;
	};
}
