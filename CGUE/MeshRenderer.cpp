#include "MeshRenderer.h"
#include <cstring>
#include "Entity.h"

namespace Engine {

	void MeshRenderer::Wire()
	{
		WIRE_COMPONENT(this->material, MaterialClass);
	}


	void MeshRenderer::Init()
	{
		static_assert(sizeof(GLfloat) == sizeof(float), "GLfloat must have same size as float.");
		assert(this->vertexData);

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

	void MeshRenderer::Render()
	{
		auto programId = this->material->GetProgramId();
		auto matrixId = glGetUniformLocation(programId, "MVP");

		glUseProgram(programId);
		glUniformMatrix4fv(matrixId, 1, GL_FALSE, &this->GetTransformation()->GetMvpMatrix()[0][0]);
		glBindVertexArray(this->vertexArray);
		glDrawArrays(GL_TRIANGLES, 0, this->numVertices);
		glUseProgram(0);
	}


	MeshRenderer::MeshRenderer()
	{
		this->vertexData = nullptr;
		this->colorData = nullptr;
		this->numVertices = 0;

		this->vertexBuffer = 0;
		this->colorBuffer = 0;
		this->vertexArray = 0;
		this->material = nullptr;
	}

	MeshRenderer::MeshRenderer(const float* bufferData, int numVertices)
	{
		this->vertexData = nullptr;
		this->numVertices = 0;

		this->vertexBuffer = 0;
		this->colorBuffer = 0;
		this->vertexArray = 0;
		this->material = nullptr;

		this->SetVertexData(bufferData, numVertices);
	}


	MeshRenderer::~MeshRenderer()
	{
		delete[] this->vertexData;
		delete[] this->colorData;


		glDeleteBuffers(1, &this->vertexArray);
		glDeleteBuffers(1, &this->colorBuffer);
		glDeleteBuffers(1, &this->vertexBuffer);
	}

	MeshRenderer *MeshRenderer::SetVertexData(const float* vertexData, int numVertices)
	{
		if (this->vertexData != nullptr)
		{
			delete[] this->vertexData;
		}
		auto arraySize = numVertices * 3;

		this->numVertices = numVertices;
		this->vertexData = new float[arraySize];
		memcpy(this->vertexData, vertexData, sizeof(*vertexData)*arraySize);

		return this;
	}

	MeshRenderer *MeshRenderer::SetVertexColorData(const float* colorData)
	{
		if (this->colorData != nullptr)
		{
			delete[] this->colorData;
		}
		auto arraySize = numVertices * 3;
		this->colorData = new float[arraySize];
		memcpy(this->colorData, colorData, sizeof(*colorData)*arraySize);

		return this;
	}
}
