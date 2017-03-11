#pragma once
#include "Renderer.h"
#include "glew/glew.h"
#include "Material.h"
#include "Transformation.h"

namespace Engine {
	class MeshRenderer :
		public Renderer
	{
		GLuint vertexBuffer;
		GLuint vertexArray;
		GLuint colorBuffer;
		Material *material;

	protected:
		float *vertexData;
		float *colorData;
		int numVertices;

	public:
		MeshRenderer();
		MeshRenderer(const float *vertexData, int numVertices);
		~MeshRenderer();

		MeshRenderer *SetVertexData(const float *vertexData, int numVertices);
		MeshRenderer *SetVertexColorData(const float *colorData);

		void Init() override;
		void Render() override;
		void Wire() override;
	};
}

