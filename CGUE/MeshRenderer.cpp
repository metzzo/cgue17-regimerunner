#include "MeshRenderer.h"
#include <cstring>
#include "Entity.h"
#include "Camera.h"

namespace Engine {
	bool MeshRenderOperation::Execute()
	{
		auto component = static_cast<MeshRenderer*>(this->GetComponent());

		auto programId = component->material->GetProgramId();

		glUseProgram(programId);

		if (component->matrixId == -2) {
			component->matrixId = glGetUniformLocation(programId, "MVP");
		}
		if (component->textureSamplerId == -2 && component->uvBuffer) {
			component->textureSamplerId = glGetUniformLocation(programId, "textureSampler");
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, component->texture->GetTextureId());
		glUniform1i(component->textureSamplerId, 0);

		auto projectionViewMatrix = component->GetEngine()->GetMainCamera()->GetProjectionViewMatrix();
		auto mvp = projectionViewMatrix * component->GetTransformation()->GetAbsoluteMatrix();

		glUniformMatrix4fv(component->matrixId, 1, GL_FALSE, &mvp[0][0]);
		glBindVertexArray(component->vertexArray);
		glDrawArrays(GL_TRIANGLES, 0, component->numVertices);
		glUseProgram(0);

		return true;
	}

	QUEUE_TYPE MeshRenderOperation::GetQueueType()
	{
		return QUEUE_RENDER_PASS;
	}

	void MeshRenderer::Wire()
	{
		WIRE_COMPONENT(this->material, MaterialClass);
		WIRE_COMPONENT(this->texture, TextureClass);
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
			0,                  // attribute 0.
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
				1,                  // attribute 1.
				3,					// size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				nullptr	            // array buffer offset
			);

			glEnableVertexAttribArray(1);
		}
		if (this->uvData)
		{
			glGenBuffers(1, &this->uvBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, this->uvBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*this->numVertices * 2, this->uvData, GL_STATIC_DRAW);
			// uv buffer
			glVertexAttribPointer(
				2,                                // attribute 2.
				2,                                // size : U+V => 2
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				nullptr							  // array buffer offset
			);

			glEnableVertexAttribArray(2);
		}

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		GetEngine()->AddOperation(new MeshRenderOperation(this));
	}

	MeshRenderer::MeshRenderer()
	{
		this->vertexData = nullptr;
		this->colorData = nullptr;
		this->uvData = nullptr;
		this->numVertices = 0;

		this->vertexBuffer = 0;
		this->colorBuffer = 0;
		this->vertexArray = 0;
		this->uvBuffer = 0;
		this->textureSamplerId = -2;
		this->matrixId = -2;

		this->texture = nullptr;
		this->material = nullptr;
	}

	MeshRenderer::MeshRenderer(const float* bufferData, int numVertices) : MeshRenderer()
	{
		this->SetVertexData(bufferData, numVertices);
	}


	MeshRenderer::~MeshRenderer()
	{
		delete[] this->vertexData;
		if (this->colorData) {
			delete[] this->colorData;
		}
		if (this->uvData) {
			delete[] this->uvData;
		}


		glDeleteBuffers(1, &this->vertexArray);
		if (this->colorBuffer) {
			glDeleteBuffers(1, &this->colorBuffer);
		}
		if (this->vertexBuffer) {
			glDeleteBuffers(1, &this->vertexBuffer);
		}
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

	MeshRenderer* MeshRenderer::SetUVData(const float* uvData)
	{
		if (this->colorData == nullptr)
		{
			auto colorData = new float[this->numVertices * 3];
			fill_n(colorData, numVertices * 3, 1.0f);
			SetVertexColorData(colorData);
			delete [] colorData;
		}

		if (this->uvData != nullptr)
		{
			delete[] this->uvData;
		}
		auto arraySize = numVertices * 2;
		this->uvData = new float[arraySize];
		memcpy(this->uvData, uvData, sizeof(*uvData)*arraySize);

		return this;
	}
}
