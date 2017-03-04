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
		OGLMaterial *material;
		Transformation *transform;
	public:
		OGLMeshRenderer();
		OGLMeshRenderer(const float *bufferData, int numVertices);
		~OGLMeshRenderer();

		void Init(GameEngine *gameEngine) override;
		void Render(GameEngine *gameEngine) override;
		void Wire() override;
	};
}

