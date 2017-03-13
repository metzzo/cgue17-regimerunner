#include "Material.h"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <vector>

namespace Engine {
	const Material MaterialClass;

	Shader::Shader(string vertexShader, string fragmentShader)
	{
		this->vertexShader = vertexShader;
		this->fragmentShader = fragmentShader;
		this->program = 0;
	}

	void Shader::Init()
	{
		// Create the shaders
		auto VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		auto FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		// Read the Vertex Shader code from the file
		string VertexShaderCode;
		ifstream VertexShaderStream(vertexShader, ios::in);
		if (VertexShaderStream.is_open()) {
			string Line = "";
			while (getline(VertexShaderStream, Line))
				VertexShaderCode += "\n" + Line;
			VertexShaderStream.close();
		}
		else {
			printf("Impossible to open %s. Are you in the right directory?\n", vertexShader.c_str());
			getchar();
			exit(1);
		}

		// Read the Fragment Shader code from the file
		string FragmentShaderCode;
		ifstream FragmentShaderStream(fragmentShader, ios::in);
		if (FragmentShaderStream.is_open()) {
			string Line = "";
			while (getline(FragmentShaderStream, Line))
				FragmentShaderCode += "\n" + Line;
			FragmentShaderStream.close();
		}
		else
		{
			printf("Impossible to open %s. Are you in the right directory?\n", fragmentShader.c_str());
			getchar();
			exit(1);
		}

		auto Result = GL_FALSE;
		int InfoLogLength;


		// Compile Vertex Shader
		printf("Compiling shader : %s\n", vertexShader.c_str());
		auto VertexSourcePointer = VertexShaderCode.c_str();
		glShaderSource(VertexShaderID, 1, &VertexSourcePointer, nullptr);
		glCompileShader(VertexShaderID);

		// Check Vertex Shader
		glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(VertexShaderID, InfoLogLength, nullptr, &VertexShaderErrorMessage[0]);
			printf("%s\n", &VertexShaderErrorMessage[0]);
		}



		// Compile Fragment Shader
		printf("Compiling shader : %s\n", fragmentShader.c_str());
		auto FragmentSourcePointer = FragmentShaderCode.c_str();
		glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, nullptr);
		glCompileShader(FragmentShaderID);

		// Check Fragment Shader
		glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(FragmentShaderID, InfoLogLength, nullptr, &FragmentShaderErrorMessage[0]);
			printf("%s\n", &FragmentShaderErrorMessage[0]);
		}



		// Link the program
		printf("Linking program\n");
		auto ProgramID = glCreateProgram();
		glAttachShader(ProgramID, VertexShaderID);
		glAttachShader(ProgramID, FragmentShaderID);
		glLinkProgram(ProgramID);

		// Check the program
		glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
		glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
			glGetProgramInfoLog(ProgramID, InfoLogLength, nullptr, &ProgramErrorMessage[0]);
			printf("%s\n", &ProgramErrorMessage[0]);
		}


		glDetachShader(ProgramID, VertexShaderID);
		glDetachShader(ProgramID, FragmentShaderID);

		glDeleteShader(VertexShaderID);
		glDeleteShader(FragmentShaderID);

		this->program = ProgramID;
	}

	Shader::~Shader()
	{
		glDeleteProgram(this->program);
	}

	GLuint Shader::GetProgramId() const
	{
		return this->program;
	}

	Material::Material()
	{
		this->renderShader = nullptr;
		this->depthShader = nullptr;
	}

	Material::~Material()
	{
		delete this->renderShader;
		delete this->depthShader;
	}

	void Material::Init()
	{
		this->renderShader->Init();
		this->depthShader->Init();
	}

	Shader* Material::GetRenderShader() const
	{
		return this->renderShader;
	}

	Shader* Material::GetDepthShader() const
	{
		return this->depthShader;
	}

	void Material::SetRenderMaterial(Shader *shader)
	{
		this->renderShader = shader;
	}

	void Material::SetDepthMaterial(Shader *shader)
	{
		this->depthShader = shader;
	}
}
