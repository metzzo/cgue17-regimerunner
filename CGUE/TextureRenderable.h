#pragma once
#include "glew/glew.h"


class TextureRenderable
{
public:
	virtual ~TextureRenderable() = default;
	virtual GLuint GetTextureId() = 0;
	virtual int GetWidth() = 0;
	virtual int GetHeight() = 0;
	virtual void Prepare()
	{
		
	}
};

