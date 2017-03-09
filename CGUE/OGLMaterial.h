#pragma once
#include <string>
#include "glew/glew.h"
#include "Material.h"

using namespace std;

namespace Engine {
	class OGLMaterial : public Material
	{
		GLuint program;
		string vertexShader;
		string fragmentShader;
	public:
		OGLMaterial();
		OGLMaterial(string vertexShader, string fragmentShader);
		~OGLMaterial();

		GLuint GetProgramId() const;

		void Init() override;
	};

	extern const OGLMaterial OGLMaterialClass;
}

