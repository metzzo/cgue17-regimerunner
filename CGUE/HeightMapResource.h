#pragma once
#include "BaseResource.h"
#include "RenderableResource.h"

namespace Engine {
	class TextureResource;

	struct HeightMapTexture
	{
		TextureResource* texture;
		float min;
		float max;
	};

	class HeightMapResource : public RenderableResource
	{
		TextureResource *heightMap;
		Mesh* mesh;
		vector<HeightMapTexture> textures;
		vec3 size;
		GLubyte* pixels;
		int texWidth;
		int texHeight;
	public:
		explicit HeightMapResource(string filename, vec3 size, int texWidth, int texHeight);
		~HeightMapResource();


		void Load() override;
		void AddTexture(TextureResource *tex, float min, float max);

		vec3 GetSize() const;
		float GetHeightAt(int x, int z) const;

		TextureResource *GetHeightMap() const;
	};

}
