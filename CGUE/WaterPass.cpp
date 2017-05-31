#include "WaterPass.h"
#include "Shader.h"
#include "GameEngine.h"
#include "Camera.h"
#include <string>

namespace Engine {

	WaterPass::WaterPass(GameEngine * gameEngine) : Pass(gameEngine)
	{
		this->shader = nullptr;

		this->shaderViewId = -2;
		this->shaderProjectionId = -2;
		this->shaderModelId = -2;
		this->shaderViewPosId = -2;
		this->shaderLightSpaceMatrixId = -2;
	}

	Engine::WaterPass::~WaterPass()
	{
	}

	void Engine::WaterPass::BeforePass()
	{
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		auto programId = shader->GetProgramId();
		DEBUG_OGL(glUseProgram(programId));

		auto cam = gameEngine->GetMainCamera();

		auto projection = cam->GetProjectionMatrix();
		auto view = cam->GetViewMatrix();
		auto viewPos = cam->GetTransformation()->GetAbsolutePosition();

		DEBUG_OGL(glUniformMatrix4fv(this->shaderProjectionId, 1, GL_FALSE, &projection[0][0]));
		DEBUG_OGL(glUniformMatrix4fv(this->shaderViewId, 1, GL_FALSE, &view[0][0]));
		DEBUG_OGL(glUniform3fv(this->shaderViewPosId, 1, &viewPos[0]));

	}

	void Engine::WaterPass::AfterPass()
	{
	}

	void Engine::WaterPass::Init()
	{
		this->shader = new Shader("materials/water.vert", "materials/water.frag");
		this->shader->Init();
		auto programId = this->shader->GetProgramId();
		this->shaderViewId = glGetUniformLocation(programId, "view");
		this->shaderProjectionId = glGetUniformLocation(programId, "projection");
		this->shaderModelId = glGetUniformLocation(programId, "model");

		this->shaderViewPosId = glGetUniformLocation(programId, "viewPos");
	}

	void Engine::WaterPass::SetDrawingTransform(Transformation * transformation)
	{
		//DEBUG_OGL(glUniformMatrix4fv(this->shaderModelId, 1, GL_FALSE, &transformation->GetAbsoluteMatrix()[0][0]));
	}
}
