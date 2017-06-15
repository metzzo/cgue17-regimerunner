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

		if (mesh->renderType != RT_SPRITE && component->GetEngine()->GetMainCamera()->BoxInFrustum(component->boxes[id]) == F_OUTSIDE && component->GetEngine()->IsCullingEnabled() || 
			(mesh->renderType == RT_SPRITE && !cam->IsHudEnabled()))
		{
			return;
		}

		auto pass = static_cast<RenderPass*>(this->GetPass());

		pass->SetDrawingTransform(component->GetTransformation());
		
		auto currentTexture = pass->GetNumShadowMaps();
		glActiveTexture(GL_TEXTURE0 + currentTexture);
		glUniform1i(pass->GetDiffuseUniform(0), currentTexture);
		glBindTexture(GL_TEXTURE_2D, mesh->diffuseTexture[0]->GetTextureId());

		if (component->GetEngine()->GetMipMappingQuality() == 0) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}

		if (component->GetEngine()->GetTextureSamplingQuality()) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}

		if (component->GetEngine()->GetMipMappingQuality() == 1) {
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		}

		if (component->GetEngine()->GetMipMappingQuality() == 2) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		}

		if (component->GetEngine()->GetMipMappingQuality() == 3) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		}

		if (component->GetEngine()->GetMipMappingQuality() == 4) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}


		if (mesh->specularTexture.size() == 1)
		{
			currentTexture++;
			glActiveTexture(GL_TEXTURE0 + currentTexture);
			glUniform1i(pass->GetSpecularUniform(0), currentTexture);
			glBindTexture(GL_TEXTURE_2D, mesh->specularTexture[0]->GetTextureId());

			if (component->GetEngine()->GetMipMappingQuality() == 0) {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			}

			if (component->GetEngine()->GetTextureSamplingQuality()) {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			else {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			}

			if (component->GetEngine()->GetMipMappingQuality() == 1) {
				glGenerateMipmap(GL_TEXTURE_2D);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			}

			if (component->GetEngine()->GetMipMappingQuality() == 2) {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			}

			if (component->GetEngine()->GetMipMappingQuality() == 3) {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			}

			if (component->GetEngine()->GetMipMappingQuality() == 4) {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			}


		} else
		{
			glUniform1i(pass->GetSpecularUniform(0), currentTexture);
		}

		if (mesh->restartIndex != -1)
		{
			DEBUG_OGL(glEnable(GL_PRIMITIVE_RESTART));
			DEBUG_OGL(glPrimitiveRestartIndex(mesh->restartIndex));
		}

		DEBUG_OGL(glUniform1i(pass->GetRenderTypeUniform(), mesh->renderType));

		DEBUG_OGL(glBindVertexArray(mesh->VAO));
		DEBUG_OGL(glDrawElements(mesh->mode, mesh->indices.size(), GL_UNSIGNED_INT, nullptr));
		DEBUG_OGL(glBindVertexArray(0));

		if (mesh->restartIndex != -1)
		{
			DEBUG_OGL(glDisable(GL_PRIMITIVE_RESTART));
		}
	}



	void DepthRenderOperation::Execute()
	{
		auto component = static_cast<Model*>(this->GetComponent());

		if (component->GetEngine()->GetMainCamera()->BoxInFrustum(component->boxes[id]) == F_OUTSIDE)
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
		DEBUG_OGL(glBindVertexArray(0));

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
			auto tmpMin = pos + mesh->min;
			auto tmpMax = pos + mesh->max;
			box.setBox(tmpMin, tmpMax.x, tmpMax.y, tmpMax.z);

			i++;
		}
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
			this->boxes.push_back(mesh->box);

			GetEngine()->GetRenderPass()->AddOperation(new MeshRenderOperation(id, mesh, this));
			if (this->resource->IsShadowCasting()) {
				GetEngine()->GetDepthPass()->AddOperation(new DepthRenderOperation(id, mesh, this));
			}

			id++;
		}

		TransformationUpdated();
	}

	Model::Model()
	{
		this->resource = nullptr;
	}

	Model::Model(RenderableResource* resource)
	{
		this->resource = resource;
	}

	Model::~Model()
	{

	}

	RenderableResource* Model::GetResource() const
	{
		return this->resource;
	}
}
