#pragma once
#include "Component.h"
#include "glew/glew.h"
#include "Texture.h"
#include "Operation.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

using namespace glm;

namespace Engine {
	class Mesh;

	class MeshRenderOperation : public Operation
	{
		Mesh *mesh;
	public:
		explicit MeshRenderOperation(Mesh *mesh, Component* component)
			: Operation(component)
		{
			this->mesh = mesh;
		}

		void Execute() override;
	};

	class DepthRenderOperation : public Operation
	{
		Mesh *mesh;
	public:
		explicit DepthRenderOperation(Mesh *mesh, Component* component)
			: Operation(component)
		{
			this->mesh = mesh;
		}

		void Execute() override;
	};

	struct Vertex {
		vec3 Position;
		vec3 Normal;
		vec2 TexCoords;
	};
	class Model;

	class Mesh
	{
		friend Model;

		GLuint VAO, VBO, EBO;

		vector<Vertex> vertices;
		vector<GLuint> indices;
		vector<GLuint> diffuseTexture;
		vector<GLuint> specularTexture;

		void SetUp();
	public:
		Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<GLuint> diffuseTexture, vector<GLuint> specularTexture);
		~Mesh();

	};

	class Model :
		public Component
	{
		friend MeshRenderOperation;
		friend DepthRenderOperation;

		vector<Mesh> meshes;

		Texture *texture;
	protected:


	public:
		Model();
		~Model();

		void Init() override;
		void Wire() override;
	};
}

