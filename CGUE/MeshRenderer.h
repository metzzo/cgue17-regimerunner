#pragma once
#include "Renderer.h"
#include "glew/glew.h"
#include "Material.h"
#include "Transformation.h"
#include "Texture.h"

namespace Engine {
	class MeshRenderer :
		public Renderer
	{
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
		void Render() override;
		void Wire() override;
	};
}

