#include "ModelResource.h"
#include <iostream>

namespace Engine {
	string ModelResource::GetDirectory() const
	{
		return filename.substr(0, filename.find_last_of('/'));;
	}

	ModelResource::ModelResource(string filename) : BaseResource(filename)
	{
		this->initialized = false;
	}


	ModelResource::~ModelResource()
	{
		for (auto mesh : this->meshes)
		{
			delete mesh;
		}
	}

	void ModelResource::Load()
	{
		// Read file via ASSIMP
		Assimp::Importer importer;
		auto scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		// Check for errors
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
			return;
		}
		// Process ASSIMP's root node recursively
		this->ProcessNode(scene->mRootNode, scene);

		for (auto mesh : this->meshes)
		{
			mesh->Init();
		}
	}

	vector<Mesh*>& ModelResource::GetMeshes()
	{
		return this->meshes;
	}

	void ModelResource::ProcessNode(aiNode* node, const aiScene* scene)
	{
		for (auto i = 0; i < node->mNumMeshes; i++)
		{
			auto mesh = scene->mMeshes[node->mMeshes[i]];
			this->meshes.push_back(this->ProcessMesh(mesh, scene));
		}
		// After we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for (auto i = 0; i < node->mNumChildren; i++)
		{
			this->ProcessNode(node->mChildren[i], scene);
		}
	}

	Mesh *ModelResource::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		// Data to fill
		auto engineMesh = new Mesh();

		// Walk through each of the mesh's vertices
		for (GLuint i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			glm::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
							  // Positions
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;
			// Normals
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
			// Texture Coordinates
			if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
				// A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else {
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			}
			// Tangent
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangent = vector;
			// Bitangent
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.Bitangent = vector;
			engineMesh->vertices.push_back(vertex);
		}
		// Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (auto i = 0; i < mesh->mNumFaces; i++)
		{
			auto face = mesh->mFaces[i];
			// Retrieve all indices of the face and store them in the indices vector
			for (auto j = 0; j < face.mNumIndices; j++) {
				engineMesh->indices.push_back(face.mIndices[j]);
			}
		}
		// Process materials
		if (mesh->mMaterialIndex >= 0)
		{
			auto material = scene->mMaterials[mesh->mMaterialIndex];
			// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
			// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
			// Same applies to other texture as the following list summarizes:
			// Diffuse: texture_diffuseN
			// Specular: texture_specularN
			// Normal: texture_normalN

			// 1. Diffuse maps
			auto diffuseMaps = this->LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			engineMesh->diffuseTexture.insert(engineMesh->diffuseTexture.end(), diffuseMaps.begin(), diffuseMaps.end());
			// 2. Specular maps
			auto specularMaps = this->LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			engineMesh->specularTexture.insert(engineMesh->specularTexture.end(), specularMaps.begin(), specularMaps.end());
			// 3. Normal maps
			auto normalMaps = this->LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
			engineMesh->normalTexture.insert(engineMesh->normalTexture.end(), normalMaps.begin(), normalMaps.end());
			// 4. Height maps
			auto heightMaps = this->LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
			engineMesh->heightTexture.insert(engineMesh->heightTexture.end(), heightMaps.begin(), heightMaps.end());
		}

		// Return a mesh object created from the extracted mesh data
		return engineMesh;
	}

	vector<TextureResource*> ModelResource::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
	{
		vector<TextureResource*> textures;
		for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
			GLboolean skip = false;
			for (GLuint j = 0; j < textures_loaded.size(); j++)
			{
				if (std::strcmp(textures_loaded[j]->GetFileName().c_str(), str.C_Str()) == 0)
				{
					textures.push_back(textures_loaded[j]);
					skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
					break;
				}
			}
			if (!skip)
			{   // If texture hasn't been loaded already, load it
				auto filename = string(str.C_Str());
				filename = GetDirectory() + '/' + filename;

				auto texture = new TextureResource(filename);
				texture->Init();
				textures.push_back(texture);
				this->textures_loaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
			}
		}
		return textures;
	}

	Mesh::Mesh()
	{
		this->VBO = 0;
		this->EBO = 0;
		this->VAO = 0;
	}

	Mesh::~Mesh()
	{
		glDeleteBuffers(1, &VAO);
		glDeleteBuffers(1, &EBO);
		glDeleteBuffers(1, &VBO);
	}

	void Mesh::Init()
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
