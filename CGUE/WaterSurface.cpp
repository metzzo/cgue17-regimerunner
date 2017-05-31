#include "WaterSurface.h"
#include "Entity.h"
#include "Camera.h"
#include "WaterPass.h"
#include "GameEngine.h"
#include "HeightMapResource.h"
#include "TextureResource.h"

namespace Engine {

	void WaterRenderOperation::Execute() {
	
		auto component = static_cast<WaterSurface*>(this->GetComponent());
		auto pass = static_cast<WaterPass*>(this->GetPass());

		pass->SetDrawingTransform(component->GetTransformation());
	
	}

	void WaterSurface::Wire() {
	
	}

	void WaterSurface::Init() {
		if (this->resource != nullptr)
		{
			this->resource->Init();
		}

		for (auto &mesh : this->resource->GetMeshes())
		{
			GetEngine()->GetRenderPass()->AddOperation(new WaterRenderOperation(mesh, this));

		}
	}

	WaterSurface::WaterSurface() {
		auto mapSize = vec3(512, 150, 512);
		this->resource = new HeightMapResource("textures/water.png", mapSize, 1024, 1024);
		this->resource->AddTexture(new TextureResource("textures/stonetext.jpg"), 0.0, 1.0);
	}

	WaterSurface::WaterSurface(int size) {
	
	}

	WaterSurface::~WaterSurface() {
	
	}

	HeightMapResource * WaterSurface::GetResource() const
	{
		return this->resource;
	}

}