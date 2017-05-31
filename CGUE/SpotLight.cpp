#include "SpotLight.h"
#include "Entity.h"
#include "GameEngine.h"
#include "RenderPass.h"

namespace Engine {
	const SpotLight SpotLightClass;

	SpotLight::SpotLight()
	{
		
	}

	SpotLight::SpotLight(float cutOff, float outerCutOff)
	{
		this->cutOff = cutOff;
		this->outerCutOff = outerCutOff;
	}
	
	SpotLight::~SpotLight()
	{
	}

	void SpotLight::SetLookAtVector(vec3 lookAt)
	{
		this->lookAtVector = lookAt;
	}


	float SpotLight::GetCutOff() const
	{
		return this->cutOff;
	}

	float SpotLight::GetOuterCutOff() const
	{
		return this->outerCutOff;
	}

	void SpotLight::Init()
	{
		this->GetEngine()->GetRenderPass()->AddSpotLight(this);
	}

	vec3 SpotLight::GetLookAtVector() const
	{
		return lookAtVector;
	}
}
