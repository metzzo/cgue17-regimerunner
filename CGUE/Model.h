#pragma once
#include "Component.h"
#include "glew/glew.h"
#include "Operation.h"
#include "Frustum.h"
#include <vector>


namespace Engine {
	class RenderableResource;
	class HeightMapResource;
	class ModelResource;
	class Mesh;

	class MeshRenderOperation : public Operation
	{
		Mesh *mesh;
		int id;
	public:
		explicit MeshRenderOperation(int id, Mesh *mesh, Component* component)
			: Operation(component)
		{
			this->mesh = mesh;
			this->id = id;
		}

		void Execute() override;
	};

	class DepthRenderOperation : public Operation
	{
		Mesh *mesh;
		int id;
	public:
		explicit DepthRenderOperation(int id, Mesh *mesh, Component* component)
			: Operation(component)
		{
			this->mesh = mesh;
			this->id = id;
		}

		void Execute() override;
	};
	
	class Model :
		public Component
	{
		friend MeshRenderOperation;
		friend DepthRenderOperation;
		bool skyBox;
		RenderableResource* resource;
		std::vector<AABox> boxes;
		bool cullingEnabled;
		float alpha;
	public:
		Model();
		explicit Model(RenderableResource *resource);
		Model(RenderableResource *resource,bool isSkyBox);
		~Model();

		void SetCullingEnabled(bool cullingEnabled);

		RenderableResource *GetResource() const;

		void Init() override;
		void Wire() override;
		void TransformationUpdated() override;
		void SetAlpha(float alpha);
	};

	extern const Model ModelClass;
}

