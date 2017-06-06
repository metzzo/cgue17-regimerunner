#include "RenderableResource.h"
#include "assimp/types.h"


namespace Engine {
	

	Mesh::Mesh()
	{
		this->VBO = 0;
		this->EBO = 0;
		this->VAO = 0;

		this->restartIndex = -1;
		this->mode = GL_TRIANGLES;
		this->renderType = RT_MODEL;
	}

	Mesh::~Mesh()
	{
		glDeleteBuffers(1, &VAO);
		glDeleteBuffers(1, &EBO);
		glDeleteBuffers(1, &VBO);
	}

	vector<Mesh*>& RenderableResource::GetMeshes()
	{
		return this->meshes;
	}

	bool RenderableResource::IsShadowCasting() const
	{
		return shadowCasting;
	}

	void RenderableResource::SetShadowCasting(bool shadowCasting)
	{
		this->shadowCasting = shadowCasting;
	}

	void Mesh::Init()
	{
		// find out box
		if (this->vertices.size() > 0) {
			min = this->vertices[0].Position;
			max = this->vertices[0].Position;

			for (auto& vertex : this->vertices)
			{
				auto pos = vertex.Position;

				if (min.x > pos.x)
				{
					min.x = pos.x;
				}
				if (min.y > pos.y)
				{
					min.y = pos.y;
				}
				if (min.z > pos.z)
				{
					min.z = pos.z;
				}
				if (max.x < pos.x)
				{
					max.x = pos.x;
				}
				if (max.y < pos.y)
				{
					max.y = pos.y;
				}
				if (max.z < pos.z)
				{
					max.z = pos.z;
				}
			}
			box.setBox(min, max.x, max.y, max.z);

		}

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
