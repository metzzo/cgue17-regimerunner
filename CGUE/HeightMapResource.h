#pragma once
#include "BaseResource.h"
#include "RenderableResource.h"

namespace Engine {
	class TextureResource;

	class HeightMapResource : public RenderableResource
	{
		TextureResource *heightMap;
		Mesh* mesh;
		vector<TextureResource*> textures;
		vec3 size;
		GLubyte* pixels;
	public:
		explicit HeightMapResource(string filename, vec3 size);
		~HeightMapResource();


		void Load() override;
		void AddTexture(TextureResource *tex);

		vec3 GetSize() const;
		float GetHeightAt(int x, int z) const;

		TextureResource *GetHeightMap() const;
	};

}
