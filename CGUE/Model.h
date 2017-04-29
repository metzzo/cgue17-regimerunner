#pragma once
#include "Component.h"
#include "glew/glew.h"
#include "Operation.h"


namespace Engine {
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
		ModelResource* resource;
	public:
		Model();
		explicit Model(ModelResource *resource);
		~Model();

		ModelResource *GetModelResource() const;

		void Init() override;
		void Wire() override;
	};

	extern const Model ModelClass;
}

