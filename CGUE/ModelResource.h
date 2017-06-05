#pragma once
#include "assimp/Importer.hpp"
#include "assimp/scene.h"

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include <vector>

#include "TextureResource.h"
#include "RenderableResource.h"

using namespace glm;
using namespace std;

namespace Engine {
	
	class ModelResource : public RenderableResource
	{
		vector<TextureResource*> textures_loaded;

		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh *ProcessMesh(aiMesh* mesh, const aiScene* scene);
		vector<TextureResource*> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);

		string GetDirectory() const;
	public:
		explicit ModelResource(string filename);
		~ModelResource();

		void Load() override;

		Mesh *GetMeshByName(string name);
		void RemoveMeshByName(string name);
	};
}

