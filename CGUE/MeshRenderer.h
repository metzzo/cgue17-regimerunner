#pragma once
#include "Renderer.h"
namespace Engine {
	class MeshRenderer :
		public Renderer
	{

	protected:
		float *bufferData;
		int numVertices;

	public:
		MeshRenderer();
		MeshRenderer(const float *bufferData, int numVertices);
		~MeshRenderer();

		void setBufferData(const float *bufferData, int numVertices);
	};
}

