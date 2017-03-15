#include "Texture.h"
#include <SDL_image.h>
#include <cassert>

namespace Engine {
	const Texture TextureClass;

	Texture::Texture()
	{
		// do NOT use this implementation on your own
		this->sdlImage = nullptr;
		this->textureId = 0;
	}

	Texture::Texture(string filename)
	{
		this->sdlImage = nullptr;
		this->filename = filename;
		this->textureId = 0;
	}


	Texture::~Texture()
	{

	}

	void Texture::Init()
	{
		this->sdlImage = IMG_Load(filename.c_str());
		if (!this->sdlImage)
		{
			GetEngine()->RaiseEngineError("Could not load image " + filename);
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

	GLuint Texture::GetTextureId() const
	{
		return this->textureId;
	}
}
