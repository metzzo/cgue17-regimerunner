#include "DirectionalLight.h"
#include "Entity.h"
#include "GameEngine.h"
#include "RenderPass.h"

namespace Engine {
	DirectionalLight::DirectionalLight()
	{
	}


	DirectionalLight::~DirectionalLight()
	{
	}

	void DirectionalLight::Init()
	{
		this->GetEngine()->GetRenderPass()->SetDirectionalLight(this);
	}


	void DirectionalLight::SetLookAtVector(vec3 lookAt)
	{
		this->lookAtVector = lookAt;
	}

	vec3 DirectionalLight::GetLookAtVector() const
	{
		return this->lookAtVector;
	}
}
