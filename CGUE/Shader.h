#pragma once
#include "Component.h"
#include <string>
#include "glew/glew.h"

using namespace std;

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
}
