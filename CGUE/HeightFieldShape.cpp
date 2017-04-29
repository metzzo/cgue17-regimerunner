#include "HeightFieldShape.h"
#include "TextureResource.h"

namespace Engine {
	HeightFieldShape::HeightFieldShape(TextureResource *resource)
	{
		this->resource = resource;
	}


	HeightFieldShape::~HeightFieldShape()
	{
	}

	void HeightFieldShape::Init()
	{
		this->resource->Load();

		PxHeightFieldDesc hfDesc;
		hfDesc.format = PxHeightFieldFormat::eS16_TM;
		hfDesc.nbColumns = resource->GetWidth();
		hfDesc.nbRows = resource->GetHeight();

		auto samples = new PxHeightFieldSample[resource->GetWidth()*resource->GetHeight()];
		auto pixels = resource->GetPixels();
		for (auto i = 0, j = 0; i < resource->GetWidth()*resource->GetHeight()*3; i += 3, j++)
		{
			samples[j].height = static_cast<PxI16*>(pixels)[i];
		}

		hfDesc.samples.data = samples;
		hfDesc.samples.stride = sizeof(PxHeightFieldSample);

		auto heightField = GetEngine()->GetCooking()->createHeightField(hfDesc,
			GetEngine()->GetPhysics()->getPhysicsInsertionCallback());

		rigidBody->SetGeometry(new PxHeightFieldGeometry(heightField, PxMeshGeometryFlags(), 1000.0f, 1000.0f, 1000.0f));
	}
}
