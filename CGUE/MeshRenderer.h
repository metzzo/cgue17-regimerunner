#pragma once
#include "Component.h"
#include "glew/glew.h"
#include "Material.h"
#include "Texture.h"
#include "Operation.h"

namespace Engine {
	class MeshRenderer;

	class MeshRenderOperation : public Operation
	{
	public:
		explicit MeshRenderOperation(Component* component)
			: Operation(component)
		{
		}

		bool Execute() override;
		QUEUE_TYPE GetQueueType() override;
	};

	class MeshRenderer :
		public Component
	{
		friend MeshRenderOperation;

		GLuint vertexBuffer;
		GLuint vertexArray;
		GLuint colorBuffer;
		GLuint uvBuffer;

		Material *material;
		Texture *texture;

		GLint matrixId;
		GLint textureSamplerId;

	protected:
		float *vertexData;
		float *colorData;
		float *uvData;
		int numVertices;

	public:
		MeshRenderer();
		MeshRenderer(const float *vertexData, int numVertices);
		~MeshRenderer();

		MeshRenderer *SetVertexData(const float *vertexData, int numVertices);
		MeshRenderer *SetVertexColorData(const float *colorData);
		MeshRenderer* SetUVData(const float *uvData);

		void Init() override;
		void Wire() override;
	};
}

