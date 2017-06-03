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
		auto pass = static_cast<RenderPass*>(this->GetPass());

		pass->SetDrawingTransform(component->GetTransformation());
		
		auto currentTexture = pass->GetNumShadowMaps();
		glActiveTexture(GL_TEXTURE0 + currentTexture);
		glUniform1i(pass->GetDiffuseUniform(0), currentTexture);
		glBindTexture(GL_TEXTURE_2D, mesh->diffuseTexture[0]->GetTextureId());

		if (mesh->specularTexture.size() == 1)
		{
			currentTexture++;
			glActiveTexture(GL_TEXTURE0 + currentTexture);
			glUniform1i(pass->GetSpecularUniform(0), currentTexture);
			glBindTexture(GL_TEXTURE_2D, mesh->specularTexture[0]->GetTextureId());
		} else
		{
			glUniform1i(pass->GetSpecularUniform(0), currentTexture);
		}

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



	void DepthRenderOperation::Execute()
	{
		auto component = static_cast<Model*>(this->GetComponent());
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

	void Model::Init()
	{
		if (this->resource != nullptr) 
		{
			this->resource->Init();
		}

		for (auto &mesh : this->resource->GetMeshes())
		{
			GetEngine()->GetRenderPass()->AddOperation(new MeshRenderOperation(mesh, this));
			GetEngine()->GetDepthPass()->AddOperation(new DepthRenderOperation(mesh, this));
		}
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
