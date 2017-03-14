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

		void Execute() override;
		OPERATION_TYPE GetOperationType() override;
	};

	class DepthRenderOperation : public Operation
	{
	public:
		explicit DepthRenderOperation(Component* component)
			: Operation(component)
		{
		}

		void Execute() override;
		OPERATION_TYPE GetOperationType() override;
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

		Material *material;
		Texture *texture;

		GLint shaderProjectionId;
		GLint shaderDiffuseTextureId;
		GLint shaderViewId;
		GLint shaderModelId;
		GLint shaderShadowMapId;
		GLint shaderLightPosId;
		GLint shaderViewPosId;
		GLint shaderLightSpaceMatrixId;

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

