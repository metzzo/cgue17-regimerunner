#include "HeightFieldGeometry.h"
#include "TextureResource.h"
#include <iostream>
#include "PxPhysicsAPI.h"
#include "GameEngine.h"
#include "RigidBody.h"

namespace Engine {
	HeightFieldGeometry::HeightFieldGeometry(TextureResource *resource, vec3 size)
	{
		this->resource = resource;
		this->size = size;
	}


	HeightFieldGeometry::~HeightFieldGeometry()
	{
	}

	PxGeometry *HeightFieldGeometry::MakeGeometry(RigidBody* rigidBody)
	{
		this->resource->Init();

		PxHeightFieldDesc hfDesc;
		hfDesc.format = PxHeightFieldFormat::eS16_TM;
		hfDesc.nbColumns = resource->GetWidth();
		hfDesc.nbRows = resource->GetHeight();

		auto samples = new PxHeightFieldSample[resource->GetWidth()*resource->GetHeight()];
		auto pixels = static_cast<GLubyte*>(resource->GetPixels());
		for (auto i = 0; i < resource->GetWidth()*resource->GetHeight(); i++)
		{
			//cout << static_cast<int>(pixels[i * resource->GetBytesPerPixel()]) << endl;
			samples[i].height = pixels[i * resource->GetBytesPerPixel()];
		}

		hfDesc.samples.data = samples;
		hfDesc.samples.stride = sizeof(PxHeightFieldSample);

		auto heightField = rigidBody->GetEngine()->GetCooking()->createHeightField(hfDesc,
			rigidBody->GetEngine()->GetPhysics()->getPhysicsInsertionCallback());

		return new PxHeightFieldGeometry(heightField, PxMeshGeometryFlags(), size.y / 255.0f, size.x / float(resource->GetWidth()), size.z / float(resource->GetHeight()));
	}
}
