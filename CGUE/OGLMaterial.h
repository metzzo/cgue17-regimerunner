#pragma once
#include <string>
#include "glew/glew.h"
#include "Material.h"

using namespace std;

namespace Engine {
	class OGLMaterial : public Material
	{
		GLuint program;

	public:
		OGLMaterial();
		OGLMaterial(string vertexShader, string fragmentShader);
		~OGLMaterial();

		void ApplyMaterial() const;
		static void UnApplyMaterial();
	};

	extern const OGLMaterial OGLMaterialClass;
}

