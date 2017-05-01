#pragma once
#include "Component.h"
#include "glew/glew.h"
#include "Operation.h"


namespace Engine {
	class RenderableResource;
	class HeightMapResource;
	class ModelResource;
	class Mesh;

	class MeshRenderOperation : public Operation
	{
		Mesh *mesh;
	public:
		explicit MeshRenderOperation(Mesh *mesh, Component* component)
			: Operation(component)
		{
			this->mesh = mesh;
		}

		void Execute() override;
	};

	class DepthRenderOperation : public Operation
	{
		Mesh *mesh;
	public:
		explicit DepthRenderOperation(Mesh *mesh, Component* component)
			: Operation(component)
		{
			this->mesh = mesh;
		}

		void Execute() override;
	};
	
	class Model :
		public Component
	{
		RenderableResource* resource;
		
	public:
		Model();
		explicit Model(RenderableResource *resource);
		~Model();

		RenderableResource *GetResource() const;

		void Init() override;
		void Wire() override;
	};

	extern const Model ModelClass;
}

