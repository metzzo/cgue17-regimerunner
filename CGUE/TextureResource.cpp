#include "TextureResource.h"
#include <SDL_image.h>
#include "GameEngine.h"

namespace Engine {
	inline bool ends_with(std::string const & value, std::string const & ending)
	{
		if (ending.size() > value.size()) return false;
		return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
	}

	TextureResource::TextureResource(string filename) : BaseResource(filename)
	{
		this->sdlImage = nullptr;
		this->textureId = 0;
	}

	TextureResource::TextureResource(GLubyte* pixels, int width, int height) : BaseResource("undefined")
	{
		this->sdlImage = SDL_CreateRGBSurfaceFrom(pixels, width, height, 24, 3 * width, 0x000000ff, 0x0000ff00, 0x00ff0000, 0);
		this->textureId = 0;
	}


	TextureResource::~TextureResource()
	{
		// TODO: free resources
	}

	void TextureResource::Load()
	{
		if (this->sdlImage == nullptr) {
			this->sdlImage = IMG_Load(filename.c_str());
			if (!this->sdlImage)
			{
				RaiseEngineError("Could not load image");
			}
		}

		DEBUG_OGL(glGenTextures(1, &this->textureId));
		DEBUG_OGL(glBindTexture(GL_TEXTURE_2D, this->textureId));

		int mode, internalMode;
		if (this->sdlImage->format->BytesPerPixel == 4) {
			internalMode = mode = GL_RGBA;
		}
		else
		{
			internalMode = mode = GL_RGB;
			if (ends_with(filename, ".bmp"))
			{
				mode = GL_BGR;
			}
		}

		DEBUG_OGL(glTexImage2D(
			GL_TEXTURE_2D, 
			0, 
			internalMode, 
			this->sdlImage->w,
			this->sdlImage->h,
			0, 
			mode,
			GL_UNSIGNED_BYTE, 
			this->sdlImage->pixels
		));

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	GLuint TextureResource::GetTextureId() const
	{
		return this->textureId;
	}

	int TextureResource::GetWidth() const
	{
		return this->sdlImage->w;
	}

	int TextureResource::GetHeight() const
	{
		return this->sdlImage->h;
	}

	void *TextureResource::GetPixels() const
	{
		return this->sdlImage->pixels;
	}

	int TextureResource::GetBytesPerPixel() const
	{
		return this->sdlImage->format->BytesPerPixel;
	}

	void TextureResource::Save(string filename) const
	{
		SDL_SaveBMP(this->sdlImage, filename.c_str());
	}
}
