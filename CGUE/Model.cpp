#include "Model.h"
#include "Entity.h"
#include "Camera.h"
#include "glm/gtx/string_cast.hpp"
#include <iostream>
#include "DepthPass.h"
#include "RenderPass.h"
#include "ModelResource.h"

namespace Engine {
	const Model ModelClass;

	void MeshRenderOperation::Execute()
	{
		auto component = static_cast<Model*>(this->GetComponent());
		auto pass = static_cast<RenderPass*>(this->GetPass());

		pass->SetDrawingTransform(component->GetTransformation());
		
		auto currentTexture = 0;
		for (auto i = 0; i < mesh->diffuseTexture.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + currentTexture);
			glUniform1i(pass->GetDiffuseUniform(i), currentTexture);
			glBindTexture(GL_TEXTURE_2D, mesh->diffuseTexture[i]->GetTextureId());
			currentTexture++;
		}
		for (auto i = 0; i < mesh->specularTexture.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + currentTexture);
			glUniform1i(pass->GetSpecularUniform(i), currentTexture);
			glBindTexture(GL_TEXTURE_2D, mesh->specularTexture[i]->GetTextureId());
			currentTexture++;
		}
		glActiveTexture(GL_TEXTURE0);

		DEBUG_OGL(glBindVertexArray(mesh->VAO));
		DEBUG_OGL(glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, nullptr));
		DEBUG_OGL(glBindVertexArray(0));

		currentTexture = 0;
		for (auto i = 0; i < mesh->diffuseTexture.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + currentTexture);
			glBindTexture(GL_TEXTURE_2D, 0);
			currentTexture++;
		}
		for (auto i = 0; i < mesh->specularTexture.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + currentTexture);
			glBindTexture(GL_TEXTURE_2D, 0);
			currentTexture++;
		}
		glActiveTexture(GL_TEXTURE0);
	}



	void DepthRenderOperation::Execute()
	{
		auto component = static_cast<Model*>(this->GetComponent());
		auto pass = static_cast<DepthPass*>(this->GetPass());

		pass->SetDrawingTransform(component->GetTransformation());

		DEBUG_OGL(glBindVertexArray(mesh->VAO));
		DEBUG_OGL(glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, nullptr));
	}


	void Model::Wire()
	{

	}

	void Model::Init()
	{
		this->resource->Init();

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

	Model::Model(ModelResource* resource)
	{
		this->resource = resource;
	}

	Model::~Model()
	{

	}

	ModelResource* Model::GetModelResource() const
	{
		return this->resource;
	}
}
