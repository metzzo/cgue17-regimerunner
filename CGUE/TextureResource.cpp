#include "TextureResource.h"
#include <SDL_image.h>
#include "GameEngine.h"
#include <iostream>

namespace Engine {
	inline bool ends_with(std::string const & value, std::string const & ending)
	{
		if (ending.size() > value.size()) return false;
		return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
	}

	TextureResource::TextureResource(string filename, bool genOgl) : BaseResource(filename)
	{
		this->sdlImage = nullptr;
		this->textureId = 0;
		this->genOgl = genOgl;
	}

	TextureResource::TextureResource(GLubyte* pixels, int width, int height, string filename, int numBytes, bool genOgl) : BaseResource(filename)
	{
		this->sdlImage = SDL_CreateRGBSurfaceFrom(pixels, width, height, numBytes == 3 ? 24 : 32, numBytes * width, 0x000000ff, 0x0000ff00, 0x00ff0000, numBytes == 3 ? 0 : 0xff000000);
		this->textureId = 0;
		this->genOgl = genOgl;
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

		if (genOgl) {
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
	}

	GLuint TextureResource::GetTextureId()
	{
		return this->textureId;
	}

	int TextureResource::GetWidth()
	{
		return this->sdlImage->w;
	}

	int TextureResource::GetHeight()
	{
		return this->sdlImage->h;
	}

	void TextureResource::Prepare()
	{
		this->Load();
	}

	void *TextureResource::GetPixels() const
	{
		if (this->sdlImage != nullptr) {
			return this->sdlImage->pixels;
		} else
		{
			return nullptr;
		}
	}

	int TextureResource::GetBytesPerPixel() const
	{
		return this->sdlImage->format->BytesPerPixel;
	}

	void TextureResource::Save(string filename) const
	{
		SDL_SaveBMP(this->sdlImage, filename.c_str());
	}

	TextureResource* TextureResource::CutoutTexture(int x, int y, int width, int height)
	{
		auto numBytes = GetBytesPerPixel();
		auto newPixels = new GLubyte[width*height*numBytes];
		auto pixels = static_cast<GLubyte*>(GetPixels());
		auto oldHeight = GetHeight();

		for (auto posx = x; posx < x + width; posx++)
		{
			for (auto posy = y; posy < y + height; posy++)
			{
				for (auto i = 0; i < numBytes; i++)
				{
					assert(posx < GetWidth());
					assert(posy < oldHeight);
					newPixels[numBytes*((posx - x)*height + (posy - y)) + i] = pixels[numBytes*(posx*oldHeight + posy) + i];
				}
			}
		}
		return new TextureResource(newPixels, width, height, GetFileName(), numBytes, false);
	}

	void TextureResource::GetRgb(int x, int y, GLubyte* r, GLubyte* g, GLubyte* b)
	{
		auto numBytes = GetBytesPerPixel();
		auto pixels = static_cast<GLubyte*>(GetPixels());
		r[0] = pixels[numBytes*(x*GetHeight() + y) + 0];
		g[0] = pixels[numBytes*(x*GetHeight() + y) + 1];
		b[0] = pixels[numBytes*(x*GetHeight() + y) + 2];
	}
}
