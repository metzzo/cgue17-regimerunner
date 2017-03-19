#include "RenderPass.h"
#include "Shader.h"
#include "GameEngine.h"
#include "SpotLight.h"
#include "Camera.h"

namespace Engine {
	RenderPass::RenderPass(GameEngine *gameEngine) : Pass(gameEngine)
	{
		this->shader = nullptr;
		this->shaderViewId = -2;
		this->shaderProjectionId = -2;
		this->shaderModelId = -2;
		this->shaderLightPosId = -2;
		this->shaderViewPosId = -2;
		this->shaderLightSpaceMatrixId = -2;
		this->shaderDiffuseTexture = -2;
		this->shaderShadowMap = -2;
	}


	RenderPass::~RenderPass()
	{
	}

	void RenderPass::BeforePass()
	{
		// TODO: handle case where no light exists
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		auto programId = shader->GetProgramId();

		DEBUG_OGL(glUseProgram(programId));

		DEBUG_OGL(glUniform1i(this->shaderDiffuseTexture, 0));
		DEBUG_OGL(glUniform1i(this->shaderShadowMap, 1));


		// TODO: handle multiple lights properly
		auto cam = gameEngine->GetMainCamera();
		// TODO: do not use global spotlight
		for (auto light : gameEngine->GetLights())
		{
			auto lightSpaceMatrix = light->GetCamera()->GetProjectionViewMatrix();
			auto lightPos = light->GetTransformation()->GetAbsolutePosition();

			DEBUG_OGL(glUniform3fv(this->shaderLightPosId, 1, &lightPos[0]));
			DEBUG_OGL(glUniformMatrix4fv(this->shaderLightSpaceMatrixId, 1, GL_FALSE, &lightSpaceMatrix[0][0]));

			DEBUG_OGL(glActiveTexture(GL_TEXTURE1));
			DEBUG_OGL(glBindTexture(GL_TEXTURE_2D, light->GetCamera()->GetTexture()));
		}

		auto projection = cam->GetProjectionMatrix();
		auto view = cam->GetViewMatrix();
		auto viewPos = cam->GetTransformation()->GetAbsolutePosition();

		DEBUG_OGL(glUniformMatrix4fv(this->shaderProjectionId, 1, GL_FALSE, &projection[0][0]));
		DEBUG_OGL(glUniformMatrix4fv(this->shaderViewId, 1, GL_FALSE, &view[0][0]));

		DEBUG_OGL(glUniform3fv(this->shaderViewPosId, 1, &viewPos[0]));

		DEBUG_OGL(glActiveTexture(GL_TEXTURE0));

	}

	void RenderPass::AfterPass()
	{
	}

	void RenderPass::Init()
	{

		this->shader = new Shader("materials/default_material.vert", "materials/default_material.frag");
		this->shader->Init();

		auto programId = this->shader->GetProgramId();
		this->shaderViewId = glGetUniformLocation(programId, "view");
		this->shaderProjectionId = glGetUniformLocation(programId, "projection");
		this->shaderModelId = glGetUniformLocation(programId, "model");
		this->shaderLightPosId = glGetUniformLocation(programId, "lightPos");
		this->shaderViewPosId = glGetUniformLocation(programId, "viewPos");
		this->shaderLightSpaceMatrixId = glGetUniformLocation(programId, "lightSpaceMatrix");
		this->shaderDiffuseTexture = glGetUniformLocation(programId, "diffuseTexture");
		this->shaderShadowMap = glGetUniformLocation(programId, "shadowMap");

	}

	void RenderPass::SetDrawingTransform(Transformation* transformation) const
	{
		DEBUG_OGL(glUniformMatrix4fv(this->shaderModelId, 1, GL_FALSE, &transformation->GetAbsoluteMatrix()[0][0]));
	}
}
