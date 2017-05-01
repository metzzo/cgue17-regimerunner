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

	class Mesh
	{
	public:
		void Init();
		Mesh();
		~Mesh();

		GLuint VAO, VBO, EBO;
		GLuint restartIndex;
		GLuint mode;

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
	public:
		vector<Mesh*>& GetMeshes();

		explicit RenderableResource(std::string filename) : BaseResource(filename)
		{
			
		}
	};
}
