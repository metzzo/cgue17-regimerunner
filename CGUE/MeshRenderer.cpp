#include "MeshRenderer.h"
#include <cstring>

namespace Engine {
	MeshRenderer::MeshRenderer()
	{
		this->bufferData = nullptr;
		this->numVertices = 0;
	}

	MeshRenderer::MeshRenderer(const float* bufferData, int numVertices)
	{
		this->bufferData = nullptr;
		this->numVertices = 0;

		this->setBufferData(bufferData, numVertices);
	}


	MeshRenderer::~MeshRenderer()
	{
		delete[] this->bufferData;
	}

	void MeshRenderer::setBufferData(const float* bufferData, int numVertices)
	{
		if (this->bufferData != nullptr)
		{
			delete this->bufferData;
		}
		auto arraySize = numVertices * 3;

		this->numVertices = numVertices;
		this->bufferData = new float[arraySize];
		memcpy(this->bufferData, bufferData, sizeof(*bufferData)*arraySize);
	}
}
