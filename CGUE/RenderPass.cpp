#include "RenderPass.h"
#include "Shader.h"
#include "GameEngine.h"
#include "SpotLight.h"
#include "Camera.h"
#include <string>
#include <sstream>

namespace Engine {
	RenderPass::RenderPass(GameEngine *gameEngine) : Pass(gameEngine)
	{
		this->lightsDirty = false;
		this->shader = nullptr;
		this->shaderViewId = -2;
		this->shaderProjectionId = -2;
		this->shaderModelId = -2;
		this->shaderLightPosId = -2;
		this->shaderViewPosId = -2;
		this->shaderLightSpaceMatrixId = -2;
		this->shaderShadowMap = -2;


		this->materialDiffuseUniform = -2;
		this->materialSpecularUniform = -2;
		this->materialShininessUniform = -2;
		this->viewPosUniform = -2;
	}


	RenderPass::~RenderPass()
	{
	}

	void RenderPass::DirtyLights()
	{
		this->lightsDirty = true;
	}

	void RenderPass::RefreshLights()
	{
		spotLightInfos.clear();

		auto programId = this->shader->GetProgramId();

		// TODO
		auto lightId = 0;
		for (auto light : spotLights)
		{
			SpotLightInfo info;
			DEBUG_OGL(info.ambientUniform		= glGetUniformLocation(programId, ("spotLights[" + to_string(lightId) + "].ambient").c_str()));
			DEBUG_OGL(info.diffuseUniform		= glGetUniformLocation(programId, ("spotLights[" + to_string(lightId) + "].diffuse").c_str()));
			DEBUG_OGL(info.specularUniform		= glGetUniformLocation(programId, ("spotLights[" + to_string(lightId) + "].specular").c_str()));
			DEBUG_OGL(info.constantUniform		= glGetUniformLocation(programId, ("spotLights[" + to_string(lightId) + "].constant").c_str()));
			DEBUG_OGL(info.linearUniform		= glGetUniformLocation(programId, ("spotLights[" + to_string(lightId) + "].linear").c_str()));
			DEBUG_OGL(info.quadraticUniform		= glGetUniformLocation(programId, ("spotLights[" + to_string(lightId) + "].quadratic").c_str()));
			DEBUG_OGL(info.cutOffUniform		= glGetUniformLocation(programId, ("spotLights[" + to_string(lightId) + "].cutOff").c_str()));
			DEBUG_OGL(info.outerCutOffUniform	= glGetUniformLocation(programId, ("spotLights[" + to_string(lightId) + "].outerCutOff").c_str()));
			DEBUG_OGL(info.directionUniform		= glGetUniformLocation(programId, ("spotLights[" + to_string(lightId) + "].direction").c_str()));
			DEBUG_OGL(info.positionUniform		= glGetUniformLocation(programId, ("spotLights[" + to_string(lightId) + "].position").c_str()));


			DEBUG_OGL(glUniform1f(info.cutOffUniform, glm::cos(glm::radians(light->GetCutOff()))));
			DEBUG_OGL(glUniform1f(info.outerCutOffUniform, glm::cos(glm::radians(light->GetOuterCutOff()))));

			DEBUG_OGL(glUniform3fv(info.ambientUniform, 1, &light->GetAmbient()[0]));
			DEBUG_OGL(glUniform3fv(info.diffuseUniform, 1, &light->GetDiffuse()[0]));
			DEBUG_OGL(glUniform3fv(info.specularUniform, 1, &light->GetSpecular()[0]));
			DEBUG_OGL(glUniform1f(info.constantUniform, light->GetConstant()));
			DEBUG_OGL(glUniform1f(info.linearUniform, light->GetLinear()));
			DEBUG_OGL(glUniform1f(info.quadraticUniform, light->GetQuadratic()));

			spotLightInfos.push_back(info);
			lightId++;
		}

		DEBUG_OGL(glUniform1i(glGetUniformLocation(programId, "numSpotLights"), lightId));
	}

	void RenderPass::BeforePass()
	{

		// TODO: handle case where no light exists
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		auto programId = shader->GetProgramId();

		DEBUG_OGL(glUseProgram(programId));

		if (this->lightsDirty)
		{
			this->RefreshLights();

			this->lightsDirty = false;
		}

		DEBUG_OGL(glUniform1i(this->materialDiffuseUniform, 0));
		DEBUG_OGL(glUniform1i(this->materialSpecularUniform, 0));
		DEBUG_OGL(glUniform1f(this->materialShininessUniform, 64.0f)); // TODO: Get Shininess from model

		//DEBUG_OGL(glUniform1i(this->shaderShadowMap, 1));


		// TODO: handle multiple lights properly
		auto cam = gameEngine->GetMainCamera();
		auto lightId = 0;
		for (auto light : spotLights)
		{
			auto lightCam = light->GetCamera();
			//auto lightSpaceMatrix = lightCam->GetProjectionViewMatrix();
			auto lightPos = light->GetTransformation()->GetAbsolutePosition();
			auto dir = lightCam->GetLookAtVector() - lightPos;

			//DEBUG_OGL(glUniformMatrix4fv(this->shaderLightSpaceMatrixId, 1, GL_FALSE, &lightSpaceMatrix[0][0]));
			DEBUG_OGL(glUniform3fv(spotLightInfos[lightId].positionUniform, 1, &lightPos[0]));
			DEBUG_OGL(glUniform3fv(spotLightInfos[lightId].directionUniform, 1, &dir[0]));



			//DEBUG_OGL(glActiveTexture(GL_TEXTURE1));
			//DEBUG_OGL(glBindTexture(GL_TEXTURE_2D, light->GetCamera()->GetTexture()));

			lightId++;
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
		this->shaderLightSpaceMatrixId = glGetUniformLocation(programId, "lightSpaceMatrix");

		this->viewPosUniform = glGetUniformLocation(programId, "viewPos");
		this->materialDiffuseUniform = glGetUniformLocation(programId, "material.diffuse");
		this->materialSpecularUniform = glGetUniformLocation(programId, "material.specular");
		this->materialShininessUniform = glGetUniformLocation(programId, "material.shininess");
	
		this->shaderLightPosId = glGetUniformLocation(programId, "lightPos");
		this->shaderViewPosId = glGetUniformLocation(programId, "viewPos");
		this->shaderShadowMap = glGetUniformLocation(programId, "shadowMap");
	}

	void RenderPass::SetDrawingTransform(Transformation* transformation) const
	{
		DEBUG_OGL(glUniformMatrix4fv(this->shaderModelId, 1, GL_FALSE, &transformation->GetAbsoluteMatrix()[0][0]));
	}

	GLint RenderPass::GetDiffuseUniform(int number) const
	{
		assert(number == 0);
		return materialDiffuseUniform;
	}

	GLint RenderPass::GetSpecularUniform(int number) const
	{
		assert(number == 0);
		return materialSpecularUniform;
	}

	void RenderPass::AddSpotLight(SpotLight* spotLight)
	{
		this->spotLights.push_back(spotLight);
	}
}
