#pragma once
#include "BaseResource.h"

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include <vector>
#include "glew/glew.h"

using namespace glm;
using namespace std;

namespace Engine {
	class TextureResource;

	struct Vertex {
		vec3 Position;
		vec3 Normal;
		vec2 TexCoords;
		vec3 Tangent;
		vec3 Bitangent;
	};

	enum MeshType
	{
		RT_MODEL = 0,
		RT_SPRITE = 1,
		RT_WATER = 2
	};

	class Mesh
	{
	public:
		void Init();
		Mesh();
		~Mesh();

		GLuint VAO, VBO, EBO;
		GLuint restartIndex;
		GLuint mode;
		GLuint renderType;

		vector<Vertex> vertices;
		vector<GLuint> indices;
		vector<TextureResource*> diffuseTexture;
		vector<TextureResource*> specularTexture;
		vector<TextureResource*> normalTexture;
		vector<TextureResource*> heightTexture;
	};


	class RenderableResource :
		public BaseResource
	{
	protected:
		vector<Mesh*> meshes;
		bool shadowCasting;
	public:
		vector<Mesh*>& GetMeshes();
		bool IsShadowCasting() const;

		explicit RenderableResource(std::string filename) : BaseResource(filename)
		{
			this->shadowCasting = true;
		}

		void SetShadowCasting(bool shadowCasting);
	};
}
