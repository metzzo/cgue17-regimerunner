#pragma once
#include "RenderableResource.h"
#include "TextureRenderable.h"

namespace Engine {
	class Camera;

	class SpriteResource :
		public RenderableResource
	{
		TextureRenderable *textureRenderable;
	public:
		explicit SpriteResource(TextureRenderable *res);
		~SpriteResource();

		void Load() override;
	};
}

