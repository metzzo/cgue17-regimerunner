#pragma once
#include "Component.h"
#include "glew/glew.h"
#include "Texture.h"
#include "Operation.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include <vector>

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
		vec3 Tangent;
		vec3 Bitangent;
	};
	class Model;

	class Mesh
	{
		friend Model;
		friend MeshRenderOperation;
		friend DepthRenderOperation;

		GLuint VAO, VBO, EBO;

		vector<Vertex> vertices;
		vector<GLuint> indices;
		vector<Texture*> diffuseTexture;
		vector<Texture*> specularTexture;
		vector<Texture*> normalTexture;
		vector<Texture*> heightTexture;

		void Init();
	public:
		Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture*> diffuseTexture, vector<Texture*> specularTexture, vector<Texture*> normalTexture, vector<Texture*> heightTexture);
		~Mesh();

	};

	class Model :
		public Component
	{
		vector<Mesh*> meshes;
		string directory;
		vector<Texture*> textures_loaded;
		string path;

		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh *ProcessMesh(aiMesh* mesh, const aiScene* scene);
		vector<Texture*> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	public:
		Model();
		explicit Model(string path);
		~Model();

		void LoadModel(string path);

		void Init() override;
		void Wire() override;
	};
}

