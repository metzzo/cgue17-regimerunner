#include "MeshRenderer.h"
#include <cstring>
#include "Entity.h"
#include "Camera.h"
#include "SpotLight.h"

namespace Engine {
	void MeshRenderOperation::Execute()
	{
		// TODO: handle case where no light exists
		auto component = static_cast<MeshRenderer*>(this->GetComponent());

		auto programId = component->material->GetRenderShader()->GetProgramId();

		DEBUG_OGL(glUseProgram(programId));

		DEBUG_OGL(glUniform1i(glGetUniformLocation(programId, "diffuseTexture"), 0));
		DEBUG_OGL(glUniform1i(glGetUniformLocation(programId, "shadowMap"), 1));

		if (component->shaderProjectionId == -2)
		{
			component->shaderProjectionId = glGetUniformLocation(programId, "projection");
		}
		if (component->shaderViewId == -2) 
		{
			component->shaderViewId = glGetUniformLocation(programId, "view");
		}
		if (component->shaderModelId == -2)
		{
			component->shaderModelId = glGetUniformLocation(programId, "model");
		}
		if (component->shaderLightPosId == -2)
		{
			component->shaderLightPosId = glGetUniformLocation(programId, "lightPos");
		}
		if (component->shaderViewPosId == -2)
		{
			component->shaderViewPosId = glGetUniformLocation(programId, "viewPos");
		}
		if (component->shaderLightSpaceMatrixId == -2)
		{
			component->shaderLightSpaceMatrixId = glGetUniformLocation(programId, "lightSpaceMatrix");
		}

		// TODO: handle multiple lights properly
		auto cam = component->GetEngine()->GetMainCamera();
		// TODO: do not use global spotlight
		auto lightMatrix = spotlight->GetTransformation()->GetAbsoluteMatrix();
		auto lightPos = -vec3(4, 3, 3);
		auto projection = cam->GetProjectionMatrix();
		auto view = cam->GetTransformation()->GetAbsoluteMatrix();
		auto viewPos = vec3(4, 3, 3);
		auto lightSpaceMatrix = spotlight->GetCamera()->GetProjectionViewMatrix();
		auto model = component->GetTransformation()->GetAbsoluteMatrix();

		DEBUG_OGL(glUniformMatrix4fv(component->shaderProjectionId, 1, GL_FALSE, &projection[0][0]));
		DEBUG_OGL(glUniformMatrix4fv(component->shaderViewId, 1, GL_FALSE, &view[0][0]));
		DEBUG_OGL(glUniformMatrix4fv(component->shaderLightSpaceMatrixId, 1, GL_FALSE, &lightSpaceMatrix[0][0]));
		DEBUG_OGL(glUniformMatrix4fv(component->shaderModelId, 1, GL_FALSE, &model[0][0]));

		DEBUG_OGL(glUniform3fv(component->shaderLightPosId, 1, &lightPos[0]));
		DEBUG_OGL(glUniform3fv(component->shaderViewPosId, 1, &viewPos[0]));


		DEBUG_OGL(glActiveTexture(GL_TEXTURE0));
		DEBUG_OGL(glBindTexture(GL_TEXTURE_2D, component->texture->GetTextureId()));

		DEBUG_OGL(glActiveTexture(GL_TEXTURE1));
		DEBUG_OGL(glBindTexture(GL_TEXTURE_2D, spotlight->GetCamera()->GetTexture()));

		DEBUG_OGL(glBindVertexArray(component->vertexArray));
		DEBUG_OGL(glDrawArrays(GL_TRIANGLES, 0, component->numVertices));
		DEBUG_OGL(glBindVertexArray(0));
	}



	OPERATION_TYPE MeshRenderOperation::GetOperationType()
	{
		return RENDER_PASS_OPERATION;
	}

	void DepthRenderOperation::Execute()
	{
		auto component = static_cast<MeshRenderer*>(this->GetComponent());
		auto programId = component->material->GetDepthShader()->GetProgramId();

		DEBUG_OGL(glUseProgram(programId));

		auto projectionViewMatrix = component->GetEngine()->GetMainCamera()->GetProjectionViewMatrix();
		auto mvp = projectionViewMatrix * component->GetTransformation()->GetAbsoluteMatrix();

		DEBUG_OGL(glUniformMatrix4fv(glGetUniformLocation(programId, "MVP"), 1, GL_FALSE, &mvp[0][0]));
		DEBUG_OGL(glBindVertexArray(component->vertexArray));
		DEBUG_OGL(glDrawArrays(GL_TRIANGLES, 0, component->numVertices));
		//glUseProgram(0);
	}

	OPERATION_TYPE DepthRenderOperation::GetOperationType()
	{
		return DEPTH_PASS_OPERATION;
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
		GLfloat vertices[] = {
			// Back face
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
			0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
			0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right
			0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,  // top-right
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom-left
			-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,// top-left
			// Front face
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
			0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
			0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // top-right
			0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
			-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
			// Left face
			-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
			-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
			-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left
			-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
			-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
			-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
			// Right face
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
			0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-right
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // top-left
			0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left
			// Bottom face
			-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
			0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-left
			0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,// bottom-left
			0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
			-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-right
			-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
			// Top face
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
			0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
			0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right
			0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
			-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f // bottom-left
		};

		DEBUG_OGL(glGenVertexArrays(1, &this->vertexArray));
		DEBUG_OGL(glGenBuffers(1, &this->vertexBuffer));
		// Fill buffer
		DEBUG_OGL(glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer));
		DEBUG_OGL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
		// Link vertex attributes
		DEBUG_OGL(glBindVertexArray(this->vertexArray));
		DEBUG_OGL(glEnableVertexAttribArray(0));
		DEBUG_OGL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0));
		DEBUG_OGL(glEnableVertexAttribArray(1));
		DEBUG_OGL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))));
		DEBUG_OGL(glEnableVertexAttribArray(2));
		DEBUG_OGL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat))));
		DEBUG_OGL(glBindBuffer(GL_ARRAY_BUFFER, 0));
		DEBUG_OGL(glBindVertexArray(0));

		/*glGenBuffers(1, &this->vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*this->numVertices * 3, this->vertexData, GL_STATIC_DRAW);

		glGenVertexArrays(1, &this->vertexArray);
		glBindVertexArray(this->vertexArray);

		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,                  // attribute 0.
			3,					// size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			nullptr	            // array buffer offset
		);

		if (this->normalData) {
			glGenBuffers(1, &this->normalBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, this->normalBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*this->numVertices * 3, this->normalData, GL_STATIC_DRAW);

			// normals
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(
				1,                  // attribute 1.
				3,					// size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				nullptr	            // array buffer offset
			);
		}

		if (this->uvData)
		{
			glGenBuffers(1, &this->uvBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, this->uvBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*this->numVertices * 2, this->uvData, GL_STATIC_DRAW);
			// uv buffer
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(
				2,                                // attribute 2.
				2,                                // size : U+V => 2
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				nullptr							  // array buffer offset
			);
		}


		/*if (this->colorData) {
			glGenBuffers(1, &this->colorBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, this->colorBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*this->numVertices * 3, this->colorData, GL_STATIC_DRAW);

			// vertex color
			glVertexAttribPointer(
				3,                  // attribute 3.
				3,					// size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				nullptr	            // array buffer offset
			);

			glEnableVertexAttribArray(3);
		}*/

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		GetEngine()->AddOperation(new MeshRenderOperation(this));
		GetEngine()->AddOperation(new DepthRenderOperation(this));
	}

	MeshRenderer::MeshRenderer()
	{
		this->vertexData = nullptr;
		this->colorData = nullptr;
		this->uvData = nullptr;
		this->normalData = nullptr;
		this->numVertices = 0;

		this->vertexBuffer = 0;
		this->colorBuffer = 0;
		this->vertexArray = 0;
		this->uvBuffer = 0;
		this->normalBuffer = 0;

		this->shaderDiffuseTextureId = -2;
		this->shaderProjectionId = -2;
		this->shaderModelId = -2;
		this->shaderViewId = -2;
		this->shaderShadowMapId = -2;
		this->shaderDiffuseTextureId = -2;
		this->shaderLightPosId = -2;
		this->shaderLightSpaceMatrixId = -2;
		this->shaderViewPosId = -2;

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
		
		if (this->normalData != nullptr)
		{
			delete[] this->normalData;
		}

		this->normalData = new float[numVertices*3];
		auto j = 0;
		auto i = 0;
		while (i < numVertices)
		{
			vec3 v1(this->vertexData[i*3], this->vertexData[i * 3 + 1], this->vertexData[i * 3 + 2]);
			i++;
			vec3 v2(this->vertexData[i * 3], this->vertexData[i * 3 + 1], this->vertexData[i * 3 + 2]);
			i++;
			vec3 v3(this->vertexData[i * 3], this->vertexData[i * 3 + 1], this->vertexData[i * 3 + 2]);
			i++;

			auto e1 = v2 - v1;
			auto e2 = v3 - v1;
			auto normal = normalize(cross(e1, e2));
			this->normalData[j] = normal.x;
			j++;
			this->normalData[j] = normal.y;
			j++;
			this->normalData[j] = normal.z;
			j++;

		}


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

	MeshRenderer* MeshRenderer::SetNormalData(const float* normalData)
	{
		if (this->normalData != nullptr)
		{
			delete[] this->normalData;
		}
		auto arraySize = numVertices * 3;
		this->normalData = new float[arraySize];
		memcpy(this->normalData, normalData, sizeof(*normalData)*arraySize);

		return this;
	}
}
