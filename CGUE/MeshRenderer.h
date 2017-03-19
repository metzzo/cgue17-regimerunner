#pragma once
#include "Component.h"
#include "glew/glew.h"
#include "Texture.h"
#include "Operation.h"

namespace Engine {
	class MeshRenderOperation : public Operation
	{
	public:
		explicit MeshRenderOperation(Component* component)
			: Operation(component)
		{
		}

		void Execute() override;
	};

	class DepthRenderOperation : public Operation
	{
	public:
		explicit DepthRenderOperation(Component* component)
			: Operation(component)
		{
		}

		void Execute() override;
	};

	class MeshRenderer :
		public Component
	{
		friend MeshRenderOperation;
		friend DepthRenderOperation;

		GLuint vertexBuffer;
		GLuint vertexArray;
		GLuint colorBuffer;
		GLuint uvBuffer;
		GLuint normalBuffer;

		Texture *texture;
	protected:
		float *vertexData;
		float *colorData;
		float *uvData;
		float *normalData;
		int numVertices;

	public:
		MeshRenderer();
		MeshRenderer(const float *vertexData, int numVertices);
		~MeshRenderer();

		MeshRenderer *SetVertexData(const float *vertexData, int numVertices);
		MeshRenderer *SetVertexColorData(const float *colorData);
		MeshRenderer* SetUVData(const float *uvData);
		MeshRenderer* SetNormalData(const float *normalData);

		void Init() override;
		void Wire() override;
	};
}

