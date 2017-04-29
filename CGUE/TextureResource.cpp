#include "TextureResource.h"
#include <SDL_image.h>

namespace Engine {
	TextureResource::TextureResource(string filename) : BaseResource(filename)
	{
		this->sdlImage = nullptr;
		this->textureId = 0;
	}


	TextureResource::~TextureResource()
	{
		// TODO: free resources
	}

	void TextureResource::Load()
	{
		this->sdlImage = IMG_Load(filename.c_str());
		if (!this->sdlImage)
		{
			return;
			RaiseEngineError("Could not load image");
		}

		glGenTextures(1, &this->textureId);
		glBindTexture(GL_TEXTURE_2D, this->textureId);

		auto Mode = GL_RGB;

		if (this->sdlImage->format->BytesPerPixel == 4) {
			Mode = GL_RGBA;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, Mode, this->sdlImage->w, this->sdlImage->h, 0, Mode, GL_UNSIGNED_BYTE, this->sdlImage->pixels);

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
}
