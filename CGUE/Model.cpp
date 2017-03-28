#include "Model.h"
#include <cstring>
#include "Entity.h"
#include "Camera.h"
#include "SpotLight.h"
#include "glm/gtx/string_cast.hpp"
#include <iostream>
#include "DepthPass.h"
#include "RenderPass.h"

namespace Engine {
	void MeshRenderOperation::Execute()
	{
		auto component = static_cast<Model*>(this->GetComponent());
		auto pass = static_cast<RenderPass*>(this->GetPass());

		pass->SetDrawingTransform(component->GetTransformation());

		DEBUG_OGL(glBindTexture(GL_TEXTURE_2D, component->texture->GetTextureId()));
		DEBUG_OGL(glBindVertexArray(component->vertexArray));
		DEBUG_OGL(glDrawArrays(GL_TRIANGLES, 0, component->numVertices));
		DEBUG_OGL(glBindVertexArray(0));
	}



	void DepthRenderOperation::Execute()
	{
		auto component = static_cast<Model*>(this->GetComponent());
		auto pass = static_cast<DepthPass*>(this->GetPass());

		pass->SetDrawingTransform(component->GetTransformation());

		DEBUG_OGL(glBindVertexArray(component->vertexArray));
		DEBUG_OGL(glDrawArrays(GL_TRIANGLES, 0, component->numVertices));
	}

	void Model::Wire()
	{
		WIRE_COMPONENT(this->texture, TextureClass);
	}

	void Model::Init()
	{
		for (auto &mesh : this->meshes)
		{
			GetEngine()->GetRenderPass()->AddOperation(new MeshRenderOperation(&mesh, this));
			GetEngine()->GetDepthPass()->AddOperation(new DepthRenderOperation(&mesh, this));
		}
	}

	Model::Model()
	{
		this->texture = nullptr;
	}


	Model::~Model()
	{
		for (auto &mesh : this->meshes)
		{
			glDeleteBuffers(1, &mesh.VAO);
			glDeleteBuffers(1, &mesh.EBO);
			glDeleteBuffers(1, &mesh.VBO);
		}

		
	}

	Mesh::Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<GLuint> diffuseTexture, vector<GLuint> specularTexture)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->diffuseTexture = diffuseTexture;
		this->specularTexture = specularTexture;

		this->VBO = 0;
		this->EBO = 0;
		this->VAO = 0;

		this->SetUp();
	}

	Mesh::~Mesh()
	{
	}

	void Mesh::SetUp()
	{
		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);
		glGenBuffers(1, &this->EBO);

		glBindVertexArray(this->VAO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex),
			&this->vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint),
			&this->indices[0], GL_STATIC_DRAW);

		// Vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			(GLvoid*)0);
		// Vertex Normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			(GLvoid*)offsetof(Vertex, Normal));
		// Vertex Texture Coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			(GLvoid*)offsetof(Vertex, TexCoords));

		glBindVertexArray(0);
	}

}
