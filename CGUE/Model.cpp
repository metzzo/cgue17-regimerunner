#include "Model.h"
#include "Entity.h"
#include "Camera.h"
#include "glm/gtx/string_cast.hpp"
#include <iostream>
#include "DepthPass.h"
#include "RenderPass.h"
#include "ModelResource.h"
#include "GameEngine.h"

namespace Engine {
	const Model ModelClass;

	void MeshRenderOperation::Execute()
	{
		assert(mesh->diffuseTexture.size() == 1);
		assert(mesh->specularTexture.size() <= 1);

		auto component = static_cast<Model*>(this->GetComponent());
		auto cam = component->GetEngine()->GetMainCamera();

		if (
			(
				component->cullingEnabled &&
				mesh->renderType != RT_SPRITE && 
				cam->BoxInFrustum(component->boxes[id]) == F_OUTSIDE && 
				component->GetEngine()->IsCullingEnabled()
			) || 
			(
				mesh->renderType == RT_SPRITE && !cam->IsHudEnabled()
			))
		{
			return;
		}

		auto pass = static_cast<RenderPass*>(this->GetPass());

		pass->SetDrawingTransform(component->GetTransformation());

		auto currentTexture = pass->GetNumShadowMaps();
		glActiveTexture(GL_TEXTURE0 + currentTexture);
		glUniform1i(pass->GetDiffuseUniform(0), currentTexture);
		glBindTexture(GL_TEXTURE_2D, mesh->diffuseTexture[0]->GetTextureId());


		if (component->GetEngine()->TextureSamplingSwitched() == true) {
			if (component->GetEngine()->GetTextureSamplingQuality()) {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			else {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			}
		}

		if (component->GetEngine()->MipMappingSwitched() == true) {
			if (component->GetEngine()->GetMipMappingQuality() == 0) {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			}

			else if (component->GetEngine()->GetMipMappingQuality() == 1) {
				glGenerateMipmap(GL_TEXTURE_2D);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			}

			else if (component->GetEngine()->GetMipMappingQuality() == 2) {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			}

			else if (component->GetEngine()->GetMipMappingQuality() == 3) {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			}

			else if (component->GetEngine()->GetMipMappingQuality() == 4) {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			}
		}


			if (mesh->specularTexture.size() == 1)
			{
				currentTexture++;
				glActiveTexture(GL_TEXTURE0 + currentTexture);
				glUniform1i(pass->GetSpecularUniform(0), currentTexture);
				glBindTexture(GL_TEXTURE_2D, mesh->specularTexture[0]->GetTextureId());

				if (component->GetEngine()->TextureSamplingSwitched() == true) {
					if (component->GetEngine()->GetTextureSamplingQuality()) {
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					}
					else {
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					}
				}

				if (component->GetEngine()->MipMappingSwitched() == true) {
					if (component->GetEngine()->GetMipMappingQuality() == 0) {
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					}

					else if (component->GetEngine()->GetMipMappingQuality() == 1) {
						glGenerateMipmap(GL_TEXTURE_2D);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
					}

					else if (component->GetEngine()->GetMipMappingQuality() == 2) {
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
					}

					else if (component->GetEngine()->GetMipMappingQuality() == 3) {
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
					}

					else if (component->GetEngine()->GetMipMappingQuality() == 4) {
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					}
				}
			}

			else {
				glUniform1i(pass->GetSpecularUniform(0), currentTexture);
			}

			if (mesh->restartIndex != -1)
			{
				DEBUG_OGL(glEnable(GL_PRIMITIVE_RESTART));
				DEBUG_OGL(glPrimitiveRestartIndex(mesh->restartIndex));
			}

			DEBUG_OGL(glUniform1i(pass->GetRenderTypeUniform(), mesh->renderType));

			if (component->alpha != 1.0f && component->GetEngine()->IsBlendingEnabled())
			{
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			}

			DEBUG_OGL(glBindVertexArray(mesh->VAO));
			DEBUG_OGL(glDrawElements(mesh->mode, mesh->indices.size(), GL_UNSIGNED_INT, nullptr));


			if (component->alpha != 1.0f && component->GetEngine()->IsBlendingEnabled())
			{
				glDisable(GL_BLEND);
			}

			if (mesh->restartIndex != -1)
			{
				DEBUG_OGL(glDisable(GL_PRIMITIVE_RESTART));
			}
		}
	



	void DepthRenderOperation::Execute()
	{
		auto component = static_cast<Model*>(this->GetComponent());

		if (
			component->alpha != 1.0f ||
			(
				component->cullingEnabled && 
				component->GetEngine()->GetMainCamera()->BoxInFrustum(component->boxes[id]) == F_OUTSIDE && 
				component->GetEngine()->IsCullingEnabled()
			))
		{
			return;
		}

		auto pass = static_cast<DepthPass*>(this->GetPass());

		pass->SetDrawingTransform(component->GetTransformation());

		if (mesh->restartIndex != -1)
		{
			DEBUG_OGL(glEnable(GL_PRIMITIVE_RESTART));
			DEBUG_OGL(glPrimitiveRestartIndex(mesh->restartIndex));
		}

		DEBUG_OGL(glBindVertexArray(mesh->VAO));
		DEBUG_OGL(glDrawElements(mesh->mode, mesh->indices.size(), GL_UNSIGNED_INT, nullptr));

		if (mesh->restartIndex != -1)
		{
			DEBUG_OGL(glDisable(GL_PRIMITIVE_RESTART));
		}
	}


	void Model::Wire()
	{

	}

	void Model::TransformationUpdated()
	{
		auto i = 0;
		auto pos = GetTransformation()->GetAbsolutePosition();
		for (auto &mesh : this->resource->GetMeshes())
		{
			auto& box = this->boxes[i];
			auto tmpMin = mesh->min;
			auto tmpMax = mesh->max;
			box.setBox(tmpMin, tmpMax.x, tmpMax.y, tmpMax.z);
			box.transform(GetTransformation()->GetAbsoluteMatrix());

			i++;
		}
	}

	void Model::SetAlpha(float alpha)
	{
		this->alpha = alpha;
	}

	void Model::Init()
	{
		if (this->resource != nullptr) 
		{
			this->resource->Init();
		}

		auto id = 0;
		for (auto &mesh : this->resource->GetMeshes())
		{
			this->boxes.push_back(AABox());

			GetEngine()->GetRenderPass()->AddOperation(new MeshRenderOperation(id, mesh, this));
			if (this->resource->IsShadowCasting()) {
				GetEngine()->GetDepthPass()->AddOperation(new DepthRenderOperation(id, mesh, this));
			}

			if (this->skyBox) {
				mesh->renderType = RT_SKYBOX;
			}

			id++;
		}

		TransformationUpdated();
	}

	Model::Model()
	{
		this->resource = nullptr;

		this->skyBox = false;
	}

	Model::Model(RenderableResource* resource,bool skybox) {
		this->resource = resource;
		this->skyBox = true;
		this->cullingEnabled = true;
		this->alpha = 1.0f;

	}

	Model::Model(RenderableResource* resource)
	{
		this->resource = resource;
		this->skyBox = false;
		this->cullingEnabled = true;
		this->alpha = 1.0f;
	}

	Model::~Model()
	{

	}

	void Model::SetCullingEnabled(bool cullingEnabled)
	{
		this->cullingEnabled = cullingEnabled;
	}

	RenderableResource* Model::GetResource() const
	{
		return this->resource;
	}
}
