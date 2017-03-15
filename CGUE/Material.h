#pragma once
#include "Component.h"
#include <string>
#include "glew/glew.h"

namespace Engine {
	class Shader
	{
		GLuint program;
		string vertexShader;
		string fragmentShader;
	public:
		Shader(string vertexShader, string fragmentShader);
		~Shader();

		GLuint GetProgramId() const;

		void Init();
	};

	class Material : public Component
	{
		Shader *renderShader;
		Shader *depthShader;
	public:
		Material();
		~Material();

		void Init() override;
		void SetRenderMaterial(Shader *shader);
		void SetDepthMaterial(Shader *shader);

		Shader *GetRenderShader() const;
		Shader *GetDepthShader() const;

	};


	extern const Material MaterialClass;
}
