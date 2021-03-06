#include "DepthPass.h"
#include "Shader.h"
#include "Camera.h"
#include "GameEngine.h"


namespace Engine {
	DepthPass::DepthPass(GameEngine* gameEngine) : Pass(gameEngine)
	{
		this->shader = nullptr;
		this->mvpUniform = -2;
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
		glCullFace(GL_FRONT);

		//glEnable(GL_POLYGON_OFFSET_FILL);
		//glPolygonOffset(4.f, 0.f);

		DEBUG_OGL(glUseProgram(this->shader->GetProgramId()));
	}

	void DepthPass::AfterPass()
	{
		DEBUG_OGL(glUseProgram(0));

		glCullFace(GL_BACK);

		DEBUG_OGL(glBindVertexArray(0));
		//glDisable(GL_POLYGON_OFFSET_FILL);
	}

	void DepthPass::Init()
	{
		this->shader = new Shader("materials/depth_material.vert", "materials/depth_material.frag");
		this->shader->Init();

		mvpUniform = glGetUniformLocation(this->shader->GetProgramId(), "MVP");
	}

	void DepthPass::SetDrawingTransform(Transformation* transformation) const
	{
		auto mvp = gameEngine->GetMainCamera()->GetProjectionMatrix() * gameEngine->GetMainCamera()->GetViewMatrix() * transformation->GetAbsoluteMatrix();

		DEBUG_OGL(glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, &mvp[0][0]));
	}
}
