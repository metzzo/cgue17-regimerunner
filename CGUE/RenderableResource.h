#pragma once
#include "BaseResource.h"

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include <vector>
#include "glew/glew.h"
#include "TextureRenderable.h"
#include "Frustum.h"

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
		RT_WATER = 2,
		RT_SKYBOX = 3
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

		vec3 min;
		vec3 max;

		string name;

		vector<Vertex> vertices;
		vector<GLuint> indices;
		vector<TextureRenderable*> diffuseTexture;
		vector<TextureRenderable*> specularTexture;
		vector<TextureRenderable*> normalTexture;
		vector<TextureRenderable*> heightTexture;
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
