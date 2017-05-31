#include "RenderPass.h"
#include "Shader.h"
#include "GameEngine.h"
#include "SpotLight.h"
#include "Camera.h"
#include "BaseLight.h"
#include "DirectionalLight.h"
#include <string>
#include <sstream>

namespace Engine {
	void LightInfo::AssignUniforms(GLuint programId, string name, int lightId, BaseLight *light)
	{
		DEBUG_OGL(this->ambientUniform = glGetUniformLocation(programId, (name + "[" + to_string(lightId) + "].ambient").c_str()));
		DEBUG_OGL(this->diffuseUniform = glGetUniformLocation(programId, (name + "[" + to_string(lightId) + "].diffuse").c_str()));
		DEBUG_OGL(this->specularUniform = glGetUniformLocation(programId, (name + "[" + to_string(lightId) + "].specular").c_str()));
		DEBUG_OGL(this->constantUniform = glGetUniformLocation(programId, (name + "[" + to_string(lightId) + "].constant").c_str()));
		DEBUG_OGL(this->linearUniform = glGetUniformLocation(programId, (name + "[" + to_string(lightId) + "].linear").c_str()));
		DEBUG_OGL(this->quadraticUniform = glGetUniformLocation(programId, (name + "[" + to_string(lightId) + "].quadratic").c_str()));
		DEBUG_OGL(this->cutOffUniform = glGetUniformLocation(programId, (name + "[" + to_string(lightId) + "].cutOff").c_str()));
		DEBUG_OGL(this->outerCutOffUniform = glGetUniformLocation(programId, (name + "[" + to_string(lightId) + "].outerCutOff").c_str()));
		DEBUG_OGL(this->directionUniform = glGetUniformLocation(programId, (name + "[" + to_string(lightId) + "].direction").c_str()));
		DEBUG_OGL(this->positionUniform = glGetUniformLocation(programId, (name + "[" + to_string(lightId) + "].position").c_str()));

		DEBUG_OGL(glUniform3fv(this->ambientUniform, 1, &light->GetAmbient()[0]));
		DEBUG_OGL(glUniform3fv(this->diffuseUniform, 1, &light->GetDiffuse()[0]));
		DEBUG_OGL(glUniform3fv(this->specularUniform, 1, &light->GetSpecular()[0]));
		DEBUG_OGL(glUniform1f(this->constantUniform, light->GetConstant()));
		DEBUG_OGL(glUniform1f(this->linearUniform, light->GetLinear()));
		DEBUG_OGL(glUniform1f(this->quadraticUniform, light->GetQuadratic()));

	}

	RenderPass::RenderPass(GameEngine *gameEngine) : Pass(gameEngine)
	{
		this->lightsDirty = false;
		this->shader = nullptr;
		this->directionalLight = nullptr;

		this->shaderViewId = -2;
		this->shaderProjectionId = -2;
		this->shaderModelId = -2;
		this->shaderViewPosId = -2;
		this->shaderLightSpaceMatrixId = -2;


		this->materialDiffuseUniform = -2;
		this->materialSpecularUniform = -2;
		this->materialShininessUniform = -2;
		this->viewPosUniform = -2;
		this->shadowMapUniform = -2;
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

		auto lightId = 0;
		for (auto light : spotLights)
		{
			LightInfo info;
			info.AssignUniforms(programId, "spotLights", lightId, light);

			DEBUG_OGL(glUniform1f(info.cutOffUniform, glm::cos(glm::radians(light->GetCutOff()))));
			DEBUG_OGL(glUniform1f(info.outerCutOffUniform, glm::cos(glm::radians(light->GetOuterCutOff()))));

			spotLightInfos.push_back(info);
			lightId++;
		}

		DEBUG_OGL(glUniform1i(glGetUniformLocation(programId, "numSpotLights"), lightId));

		if (this->directionalLight != nullptr)
		{
			this->directionalLightInfo.AssignUniforms(programId, "dirLights", 0, this->directionalLight);
		}
		DEBUG_OGL(glUniform1i(glGetUniformLocation(programId, "numDirLights"), this->directionalLight != nullptr ? 1 : 0));
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

		// TODO: handle multiple lights properly
		auto cam = gameEngine->GetMainCamera();
		auto lightId = 0;
		for (auto light : spotLights)
		{
			auto lightPos = light->GetTransformation()->GetAbsolutePosition();
			auto dir = light->GetLookAtVector() - lightPos;

			DEBUG_OGL(glUniform3fv(spotLightInfos[lightId].positionUniform, 1, &lightPos[0]));
			DEBUG_OGL(glUniform3fv(spotLightInfos[lightId].directionUniform, 1, &dir[0]));

			lightId++;
		}

		if (directionalLight != nullptr)
		{
			auto lightSpaceMatrix = directionalLight->GetCamera()->GetProjectionViewMatrix();
			auto lightPos = directionalLight->GetTransformation()->GetAbsolutePosition();
			auto dir = directionalLight->GetCamera()->GetLookAtVector() - lightPos;

			DEBUG_OGL(glUniformMatrix4fv(this->shaderLightSpaceMatrixId, 1, GL_FALSE, &lightSpaceMatrix[0][0]));
			DEBUG_OGL(glUniform3fv(directionalLightInfo.positionUniform, 1, &lightPos[0]));
			DEBUG_OGL(glUniform3fv(directionalLightInfo.directionUniform, 1, &dir[0]));

			DEBUG_OGL(glUniform1i(shadowMapUniform, 1));

			DEBUG_OGL(glActiveTexture(GL_TEXTURE1));
			DEBUG_OGL(glBindTexture(GL_TEXTURE_2D, directionalLight->GetCamera()->GetTexture()));
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
		this->shadowMapUniform = glGetUniformLocation(programId, "shadowMap");
	
	
		this->shaderViewPosId = glGetUniformLocation(programId, "viewPos");
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
		this->DirtyLights();
	}

	void RenderPass::SetDirectionalLight(DirectionalLight* directionalLight)
	{
		this->directionalLight = directionalLight;
		this->DirtyLights();
	}
}
