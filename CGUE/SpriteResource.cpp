#include "SpriteResource.h"

namespace Engine {
	SpriteResource::SpriteResource(TextureRenderable* res) : RenderableResource("")
	{
		this->textureRenderable = res;
	}

	SpriteResource::~SpriteResource()
	{
	}

	void SpriteResource::Load()
	{
		this->textureRenderable->Prepare();

		this->shadowCasting = false;

		auto mesh = new Mesh();
		
		auto w = this->textureRenderable->GetWidth() / 2;
		auto h = this->textureRenderable->GetHeight() / 2;

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
		mesh->indices.push_back(1);

		mesh->diffuseTexture.push_back(this->textureRenderable);
		mesh->renderType = RT_SPRITE;

		mesh->Init();
		this->meshes.push_back(mesh);
	}
}