#pragma once
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include <vector>

#include "Texture.h"

using namespace glm;

namespace Engine {
	struct Vertex {
		vec3 Position;
		vec3 Normal;
		vec2 TexCoords;
		vec3 Tangent;
		vec3 Bitangent;
	};

	class ModelResource;

	class Mesh
	{
		friend ModelResource;

		void Init();
		Mesh();
		~Mesh();

	public:
		GLuint VAO, VBO, EBO;

		vector<Vertex> vertices;
		vector<GLuint> indices;
		vector<Texture*> diffuseTexture;
		vector<Texture*> specularTexture;
		vector<Texture*> normalTexture;
		vector<Texture*> heightTexture;
	};

	class ModelResource
	{
		vector<Mesh*> meshes;
		string directory;
		vector<Texture*> textures_loaded;
		string path;

		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh *ProcessMesh(aiMesh* mesh, const aiScene* scene);
		vector<Texture*> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);

		bool initialized;
	public:
		explicit ModelResource(string path);
		~ModelResource();

		void Init();

		vector<Mesh*>& GetMeshes();
	};
}

