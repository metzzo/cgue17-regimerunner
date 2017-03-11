#pragma once
#include "Renderer.h"
namespace Engine {
	class MeshRenderer :
		public Renderer
	{

	protected:
		float *vertexData;
		float *colorData;
		int numVertices;

	public:
		MeshRenderer();
		MeshRenderer(const float *vertexData, int numVertices);
		~MeshRenderer();

		void SetVertexData(const float *vertexData, int numVertices);
		void SetVertexColorData(const float *colorData);
	};
}

