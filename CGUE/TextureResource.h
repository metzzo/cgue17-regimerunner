#pragma once
#include "Component.h"
#include "packages/sdl2.v140.2.0.4/build/native/include/SDL_surface.h"
#include "glew/glew.h"
#include "BaseResource.h"
#include "TextureRenderable.h"
#include "glm/detail/type_vec2.hpp"

using namespace std;

namespace Engine {
	class TextureResource : public BaseResource, public TextureRenderable
	{
		SDL_Surface* sdlImage;
		GLuint textureId;
		bool genOgl;
	public:
		explicit TextureResource(string filename, bool genOgl = true);
		explicit TextureResource(GLubyte *pixels, int width, int height, string filename = "undefined", int numBytes = 3, bool genOgl = true);
		~TextureResource();

		void Load() override;

		GLuint GetTextureId() override;
		int GetWidth() override;
		int GetHeight() override;
		void Prepare() override;

		void* GetPixels() const;
		int GetBytesPerPixel() const;
		void Save(string filename) const;

		TextureResource *CutoutTexture(int x, int y, int width, int height);
		void GetRgb(int x, int y, GLubyte *r, GLubyte* g, GLubyte* b);
	};
}
