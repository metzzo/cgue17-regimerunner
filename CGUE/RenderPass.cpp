#include "RenderPass.h"
#include "Shader.h"
#include "GameEngine.h"
#include "SpotLight.h"
#include "Camera.h"
#include "BaseLight.h"
#include "DirectionalLight.h"
#include <string>
#include <sstream>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

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
		DEBUG_OGL(this->shadowCastingUniform = glGetUniformLocation(programId, (name + "[" + to_string(lightId) + "].shadowCasting").c_str()));
		DEBUG_OGL(this->shadowMapIndexUniform = glGetUniformLocation(programId, (name + "[" + to_string(lightId) + "].shadowMapIndex").c_str()));
		DEBUG_OGL(this->spaceMatrixUniform = glGetUniformLocation(programId, (name + "[" + to_string(lightId) + "].spaceMatrix").c_str()));

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

		this->viewUniform = -2;
		this->projectionUniform = -2;
		this->hudProjectionUniform = -2;
		this->modelUniform = -2;
		this->shaderViewPosId = -2;

		this->WaterNormalMapUniform = -2;
		this->WaterUVDVMapUniform = -2;
		this->WaterReflectionUniform = -2;
		this->EyeTanSpaceUniform =-2;
		this->LightTanSpaceUniform = -2;
		this->waveOffsetUniform = -2;
		this->texOffsetUniform = -2;

		this->materialDiffuseUniform = -2;
		this->materialSpecularUniform = -2;
		this->materialShininessUniform = -2;
		this->viewPosUniform = -2;

		this->numShadowMaps = 0;
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
		auto shadowMapIndex = 0;
		for (auto light : spotLights)
		{
			LightInfo info;
			info.AssignUniforms(programId, "spotLights", lightId, light);

			DEBUG_OGL(glUniform1f(info.cutOffUniform, glm::cos(glm::radians(light->GetCutOff()))));
			DEBUG_OGL(glUniform1f(info.outerCutOffUniform, glm::cos(glm::radians(light->GetOuterCutOff()))));
			DEBUG_OGL(glUniform1i(info.shadowCastingUniform, light->IsShadowCasting()));
			if (light->IsShadowCasting())
			{
				DEBUG_OGL(glUniform1i(info.shadowMapIndexUniform, shadowMapIndex));
				shadowMapIndex++;
			}

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
		glCullFace(GL_BACK);

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
		auto utilitycam = gameEngine->GetUtilityCamera();
		auto lightId = 0;
		auto shadowMapIndex = 0;
		for (auto light : spotLights)
		{
			auto lightPos = light->GetTransformation()->GetAbsolutePosition();
			auto dir = light->GetCamera()->GetLookAtVector() - lightPos;

			DEBUG_OGL(glUniform3fv(spotLightInfos[lightId].positionUniform, 1, &lightPos[0]));
			DEBUG_OGL(glUniform3fv(spotLightInfos[lightId].directionUniform, 1, &dir[0]));

			if (light->IsShadowCasting())
			{
				auto lightSpaceMatrix = light->GetCamera()->GetProjectionMatrix() * light->GetCamera()->GetViewMatrix();

				DEBUG_OGL(glUniformMatrix4fv(spotLightInfos[lightId].spaceMatrixUniform, 1, GL_FALSE, &lightSpaceMatrix[0][0]));

				DEBUG_OGL(glUniform1i(shadowMapUniform[shadowMapIndex],  shadowMapIndex));

				DEBUG_OGL(glActiveTexture(GL_TEXTURE0 + shadowMapIndex));
				DEBUG_OGL(glBindTexture(GL_TEXTURE_2D, light->GetCamera()->GetTextureId()));

				shadowMapIndex++;
			}

			lightId++;
		}
		this->numShadowMaps = shadowMapIndex;

		// REFLECTION TEXTURE FROM SECOND CAMERA
		DEBUG_OGL(glUniform1i(this->GetWaterReflectionUniform(), 16));
		DEBUG_OGL(glActiveTexture(GL_TEXTURE0 + 16));
		DEBUG_OGL(glBindTexture(GL_TEXTURE_2D, utilitycam->GetTextureId()));

		DEBUG_OGL(glUniform3fv(this->EyeTanSpaceUniform,1, glm::value_ptr(glm::vec3(cam->GetTransformation()->GetAbsolutePosition()))));

		if (directionalLight != nullptr)
		{
			auto lightPos = directionalLight->GetTransformation()->GetAbsolutePosition();
			auto dir = directionalLight->GetLookAtVector() - lightPos;
			DEBUG_OGL(glUniform3fv(this->LightTanSpaceUniform, 1, glm::value_ptr(dir)));
			DEBUG_OGL(glUniform3fv(directionalLightInfo.directionUniform, 1, &dir[0]));
		}

		auto projection = cam->GetProjectionMatrix();
		auto hudProjection = cam->GetHudProjectionMatrix();
		auto view = cam->GetViewMatrix();
		auto viewPos = cam->GetTransformation()->GetAbsolutePosition();

		DEBUG_OGL(glUniformMatrix4fv(this->projectionUniform, 1, GL_FALSE, &projection[0][0]));
		DEBUG_OGL(glUniformMatrix4fv(this->hudProjectionUniform, 1, GL_FALSE, &hudProjection[0][0]));
		DEBUG_OGL(glUniformMatrix4fv(this->viewUniform, 1, GL_FALSE, &view[0][0]));

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
		this->viewUniform = glGetUniformLocation(programId, "view");
		this->projectionUniform = glGetUniformLocation(programId, "projection");
		this->hudProjectionUniform = glGetUniformLocation(programId, "hudProjection");
		this->modelUniform = glGetUniformLocation(programId, "model");
		this->renderTypeUniform = glGetUniformLocation(programId, "renderType");

		this->WaterNormalMapUniform = glGetUniformLocation(programId,"normalSampler");
		this->WaterUVDVMapUniform = glGetUniformLocation(programId, "displaceSampler");
		this->WaterReflectionUniform = glGetUniformLocation(programId, "reflectSampler");
		this->EyeTanSpaceUniform = glGetUniformLocation(programId, "eyeTanSpace");
		this->LightTanSpaceUniform = glGetUniformLocation(programId, "lightTanSpace");

		this->waveOffsetUniform = glGetUniformLocation(programId, "waveOffset");
		this->texOffsetUniform = glGetUniformLocation(programId, "texOffset");

		this->viewPosUniform = glGetUniformLocation(programId, "viewPos");
		this->materialDiffuseUniform = glGetUniformLocation(programId, "material.diffuse");
		this->materialSpecularUniform = glGetUniformLocation(programId, "material.specular");
		this->materialShininessUniform = glGetUniformLocation(programId, "material.shininess");
		for (auto i = 0; i < NUM_SHADOW_MAPS; i++) {
			DEBUG_OGL(this->shadowMapUniform[i] = glGetUniformLocation(programId, ("shadowMap" + to_string(i)).c_str()));
		}
	
	
		DEBUG_OGL(this->shaderViewPosId = glGetUniformLocation(programId, "viewPos"));
	}

	void RenderPass::SetDrawingTransform(Transformation* transformation) const
	{
		DEBUG_OGL(glUniformMatrix4fv(this->modelUniform, 1, GL_FALSE, &transformation->GetAbsoluteMatrix()[0][0]));
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

	int RenderPass::GetNumShadowMaps() const
	{
		return this->numShadowMaps;
	}

	GLint RenderPass::GetWaterNormalMapUniform() const
	{
		return this->WaterNormalMapUniform;
	}

	GLint RenderPass::GetWaterUVDVMapUniform() const
	{
		return this->WaterUVDVMapUniform;
	}

	GLint RenderPass::GetWaterReflectionUniform() const
	{
		return this->WaterReflectionUniform;
	}

	GLint RenderPass::GetWaveOffsetUniform() const
	{
		return this->waveOffsetUniform;
	}

	GLint RenderPass::GetTexOffsetUniform() const
	{
		return this->texOffsetUniform;
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

	GLint RenderPass::GetRenderTypeUniform() const
	{
		return renderTypeUniform;
	}
	
	GLint RenderPass::GetArrayUniformLocation(int id, string name) {
		return glGetUniformLocation(this->shader->GetProgramId(), (name + "[" + to_string(id) + "]").c_str());
	}
}
