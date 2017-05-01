#include "ConvexShape.h"
#include "Entity.h"
#include "Model.h"
#include "RenderableResource.h"
#include <iostream>
#include "PxPhysicsAPI.h"
#include "GameEngine.h"

namespace Engine {

	ConvexShape::ConvexShape()
	{
		this->model = nullptr;
		this->body = nullptr;
	}


	ConvexShape::~ConvexShape()
	{
	}

	void ConvexShape::Wire()
	{
		BaseShape::Wire();
		WIRE_COMPONENT(this->model, ModelClass);
	}


	void ConvexShape::Init()
	{
		auto resource = this->model->GetResource();
		auto meshes = resource->GetMeshes();
		assert(meshes.size() == 1); // TODO: support submeshes

		auto mesh = meshes.front();
		auto convexVerts = new PxVec3[mesh->vertices.size()];
		auto i = 0;
		for (auto &vert : mesh->vertices)
		{
			convexVerts[i].x = vert.Position.x;
			convexVerts[i].y = vert.Position.y;
			convexVerts[i].z = vert.Position.z;
			
			i++;
		}

		PxConvexMeshDesc convexDesc;
		convexDesc.points.count = mesh->vertices.size();
		convexDesc.points.stride = sizeof(PxVec3);
		convexDesc.points.data = convexVerts;
		convexDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX;
		
		cout << "Bake Convexmesh" << endl;
		auto convexMesh = GetEngine()->GetCooking()->createConvexMesh(convexDesc, GetEngine()->GetPhysics()->getPhysicsInsertionCallback());
		rigidBody->SetGeometry(new PxConvexMeshGeometry(convexMesh));
	}
}
