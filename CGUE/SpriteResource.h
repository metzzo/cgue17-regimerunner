#pragma once
#include "RenderableResource.h"
#include "Camera.h"

namespace Engine {
	class SpriteResource :
		public RenderableResource
	{
		TextureResource *textureResource;
		Camera *camera;
	public:
		explicit SpriteResource(TextureResource *res);
		explicit SpriteResource(Camera *cam);
		~SpriteResource();

		void Load() override;
	};
}

