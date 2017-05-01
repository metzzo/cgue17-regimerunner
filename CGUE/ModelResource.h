#pragma once
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include <vector>

#include "TextureResource.h"
#include "BaseResource.h"

using namespace glm;
using namespace std;

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
		vector<TextureResource*> diffuseTexture;
		vector<TextureResource*> specularTexture;
		vector<TextureResource*> normalTexture;
		vector<TextureResource*> heightTexture;
	};

	class ModelResource : public BaseResource
	{
		vector<Mesh*> meshes;
		vector<TextureResource*> textures_loaded;

		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh *ProcessMesh(aiMesh* mesh, const aiScene* scene);
		vector<TextureResource*> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);

		bool initialized;

		string GetDirectory() const;
	public:
		explicit ModelResource(string filename);
		~ModelResource();

		void Load() override;

		vector<Mesh*>& GetMeshes();
	};
}

