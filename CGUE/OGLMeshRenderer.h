#pragma once
#include "MeshRenderer.h"
#include "glew/glew.h"
#include "OGLMaterial.h"
#include "Transformation.h"

namespace Engine {
	class OGLMeshRenderer : public MeshRenderer
	{
		GLuint vertexBuffer;
		GLuint vertexArray;
		GLuint colorBuffer;
		OGLMaterial *material;
	public:
		OGLMeshRenderer();
		OGLMeshRenderer(const float *bufferData, int numVertices);
		~OGLMeshRenderer();

		void Init() override;
		void Render() override;
		void Wire() override;
	};
}

