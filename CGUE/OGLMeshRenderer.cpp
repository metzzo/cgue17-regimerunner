#include "OGLMeshRenderer.h"
#include "Entity.h"
#include <typeinfo>

namespace Engine {
	OGLMeshRenderer::OGLMeshRenderer() : MeshRenderer()
	{
		this->vertexBuffer = 0;
		this->colorBuffer = 0;
		this->vertexArray = 0;
		this->material = nullptr;
	}

	OGLMeshRenderer::OGLMeshRenderer(const float* bufferData, int numVertices) : MeshRenderer(bufferData, numVertices)
	{
		this->vertexBuffer = 0;
		this->colorBuffer = 0;
		this->vertexArray = 0;
		this->material = nullptr;
	}

	OGLMeshRenderer::~OGLMeshRenderer()
	{
		glDeleteBuffers(1, &this->vertexArray);
		glDeleteBuffers(1, &this->colorBuffer);
		glDeleteBuffers(1, &this->vertexBuffer);
	}

	void OGLMeshRenderer::Wire()
	{
		WIRE_COMPONENT(this->material, OGLMaterialClass);
	}


	void OGLMeshRenderer::Init()
	{
		static_assert(sizeof(GLfloat) == sizeof(float), "GLfloat must have same size as float.");
		assert(this->vertexData);

		MeshRenderer::Init();

		glGenBuffers(1, &this->vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*this->numVertices * 3, this->vertexData, GL_STATIC_DRAW);

		glGenVertexArrays(1, &this->vertexArray);
		glBindVertexArray(this->vertexArray);
		// vertex positions
		
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,					// size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			nullptr	            // array buffer offset
		);
		glEnableVertexAttribArray(0);

		if (this->colorData) {
			glGenBuffers(1, &this->colorBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, this->colorBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*this->numVertices * 3, this->colorData, GL_STATIC_DRAW);

			// vertex color
			glVertexAttribPointer(
				1,                  // attribute 1. No particular reason for 0, but must match the layout in the shader.
				3,					// size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				nullptr	            // array buffer offset
			);

			glEnableVertexAttribArray(1);
		}

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OGLMeshRenderer::Render()
	{
		auto programId = this->material->GetProgramId();
		auto matrixId = glGetUniformLocation(programId, "MVP");

		glUseProgram(programId);
		glUniformMatrix4fv(matrixId, 1, GL_FALSE, &this->GetTransformation()->GetMvpMatrix()[0][0]);
		glBindVertexArray(this->vertexArray);
		glDrawArrays(GL_TRIANGLES, 0, this->numVertices);
		glUseProgram(0);
	}
}
