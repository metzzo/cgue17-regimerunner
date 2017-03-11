#pragma once
#include "Component.h"
#include <string>
#include "glew/glew.h"

namespace Engine {
	class Material : public Component
	{
		GLuint program;
		string vertexShader;
		string fragmentShader;
	public:
		Material();
		Material(string vertexShader, string fragmentShader);
		~Material();

		GLuint GetProgramId() const;

		void Init() override;
	};


	extern const Material MaterialClass;
}
