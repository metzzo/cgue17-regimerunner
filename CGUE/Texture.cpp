#include "Texture.h"
#include <SDL_image.h>

namespace Engine {
	Texture::Texture()
	{
		// do NOT use this implementation on your own
	}

	Texture::Texture(string filename)
	{
		this->sdlImage = IMG_Load(filename.c_str());
	}


	Texture::~Texture()
	{

	}
}