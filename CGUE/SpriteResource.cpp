#include "SpriteResource.h"
#include "TextureResource.h"

namespace Engine {
	SpriteResource::SpriteResource(TextureResource* res): RenderableResource("")
	{
		this->textureResource = res;
		this->camera = nullptr;
	}

	SpriteResource::SpriteResource(Camera* cam): RenderableResource("")
	{
		this->camera = cam;
		this->textureResource = nullptr;
	}

	SpriteResource::~SpriteResource()
	{
	}

	void SpriteResource::Load()
	{
		if (this->textureResource)
		{
			this->textureResource->Init();
		}

		this->shadowCasting = false;

		auto mesh = new Mesh();
		
		auto w = this->textureResource->GetWidth() / 2;
		auto h = this->textureResource->GetHeight() / 2;

		Vertex v;
		v.Position = vec3(-1*w, 1*h, 0);
		v.TexCoords = vec2(0, 1);
		mesh->vertices.push_back(v);

		v.Position = vec3(-1*w, -1*h, 0);
		v.TexCoords = vec2(0, 0);
		mesh->vertices.push_back(v);

		v.Position = vec3(1*w, 1*h, 0);
		v.TexCoords = vec2(1, 1);
		mesh->vertices.push_back(v);

		v.Position = vec3(1*w, -1*h, 0);
		v.TexCoords = vec2(1, 0);
		mesh->vertices.push_back(v);

		mesh->indices.push_back(0);
		mesh->indices.push_back(1);
		mesh->indices.push_back(2);
		mesh->indices.push_back(2);
		mesh->indices.push_back(3);
		mesh->indices.push_back(0);

		mesh->diffuseTexture.push_back(this->textureResource); // TODO: also for camera support
		mesh->renderType = RT_SPRITE;

		mesh->Init();
		this->meshes.push_back(mesh);
	}
}