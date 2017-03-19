#include "DepthPass.h"
#include "Shader.h"
#include "Camera.h"


namespace Engine {
	DepthPass::DepthPass(GameEngine* gameEngine) : Pass(gameEngine)
	{
		this->shader = nullptr;
	}

	DepthPass::~DepthPass()
	{
		if (this->shader)
		{
			// TODO delete shader
			delete this->shader;
		}
	}

	void DepthPass::BeforePass()
	{
		glClear(GL_DEPTH_BUFFER_BIT);

		DEBUG_OGL(glUseProgram(this->shader->GetProgramId()));
	}

	void DepthPass::AfterPass()
	{
		DEBUG_OGL(glUseProgram(0));
	}

	void DepthPass::Init()
	{
		this->shader = new Shader("materials/depth_material.vert", "materials/depth_material.frag");
	}

	void DepthPass::SetDrawingTransform(Transformation* transformation) const
	{
		auto projectionViewMatrix = gameEngine->GetMainCamera()->GetProjectionViewMatrix();
		auto mvp = projectionViewMatrix * transformation->GetAbsoluteMatrix();

		DEBUG_OGL(glUniformMatrix4fv(glGetUniformLocation(this->shader->GetProgramId(), "MVP"), 1, GL_FALSE, &mvp[0][0]));
	}
}
