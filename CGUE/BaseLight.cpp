#include "BaseLight.h"
#include "GameEngine.h"
#include "RenderPass.h"

namespace Engine {
	void BaseLight::RefreshRenderPass() const
	{
		if (this->GetEntity() && this->GetEngine()) {
			this->GetEngine()->GetRenderPass()->DirtyLights();
		}
	}

	BaseLight::BaseLight()
	{
		this->constant = 1.0f;
		this->linear = 0.09f;
		this->quadratic = 0.032f;
	}


	BaseLight::~BaseLight()
	{
	}

	void BaseLight::AttachedToEntity()
	{
		this->RefreshRenderPass(); // render pass needs to set up light stuff again
	}

	void BaseLight::SetAmbient(vec3 ambient)
	{
		this->ambient = ambient;
		this->RefreshRenderPass();
	}

	void BaseLight::SetDiffuse(vec3 diffuse)
	{
		this->diffuse = diffuse;
		this->RefreshRenderPass();
	}

	void BaseLight::SetSpecular(vec3 specular)
	{
		this->specular = specular;
		this->RefreshRenderPass();
	}

	void BaseLight::SetConstant(float val)
	{
		this->constant = val;
		this->RefreshRenderPass();
	}

	void BaseLight::SetLinear(float val)
	{
		this->linear = val;
		this->RefreshRenderPass();
	}

	void BaseLight::SetQuadratic(float val)
	{
		this->quadratic = val;
		this->RefreshRenderPass();
	}

	vec3 BaseLight::GetAmbient() const
	{
		return this->ambient;
	}

	vec3 BaseLight::GetDiffuse() const
	{
		return this->diffuse;
	}

	vec3 BaseLight::GetSpecular() const
	{
		return this->specular;
	}

	float BaseLight::GetConstant() const
	{
		return this->constant;
	}

	float BaseLight::GetLinear() const
	{
		return this->linear;
	}

	float BaseLight::GetQuadratic() const
	{
		return this->quadratic;
	}
}
