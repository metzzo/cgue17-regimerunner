#include "OGLMeshRenderer.h"
#include "Entity.h"
#include <typeinfo>

namespace Engine {
	OGLMeshRenderer::OGLMeshRenderer() : MeshRenderer()
	{
		this->vertexBuffer = 0;
		this->material = nullptr;
		this->transform = nullptr;
	}

	OGLMeshRenderer::OGLMeshRenderer(const float* bufferData, int numVertices) : MeshRenderer(bufferData, numVertices)
	{
		this->vertexBuffer = 0;
		this->material = nullptr;
		this->transform = nullptr;
	}

	OGLMeshRenderer::~OGLMeshRenderer()
	{
		glDeleteBuffers(1, &this->vertexBuffer);
	}

	void OGLMeshRenderer::Wire()
	{
		WIRE_COMPONENT(this->transform, TransformationClass);
		WIRE_COMPONENT(this->material, OGLMaterialClass);
	}


	void OGLMeshRenderer::Init(GameEngine* gameEngine)
	{
		static_assert(sizeof(GLfloat) == sizeof(float), "GLfloat must have same size as float.");

		MeshRenderer::Init(gameEngine);

		glGenBuffers(1, &this->vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*this->numVertices * 3, this->bufferData, GL_STATIC_DRAW);


		glGenVertexArrays(1, &this->vertexArray);
		glBindVertexArray(this->vertexArray);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			this->numVertices,  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0           // array buffer offset
		);

		glBindVertexArray(0);
	}

	void OGLMeshRenderer::Render(GameEngine* gameEngine)
	{
		this->material->ApplyMaterial();
		glBindVertexArray(this->vertexArray);
		glDrawArrays(GL_TRIANGLES, 0, this->numVertices);
	}
}
