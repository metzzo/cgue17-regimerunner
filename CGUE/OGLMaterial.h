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

		void Init() override;

		void ApplyMaterial() const;
		static void UnApplyMaterial();
	};

	extern const OGLMaterial OGLMaterialClass;
}

