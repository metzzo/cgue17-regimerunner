#include "SpotLight.h"
#include "Entity.h"
#include "GameEngine.h"
#include "RenderPass.h"
#include "DepthPass.h"

namespace Engine {
	SpotLight::SpotLight(mat4 projectionMatrix, int shadowMapSize, float cutOff, float outerCutOff)
	{
		this->shadowMapSize = shadowMapSize;
		this->camera = nullptr;
		this->projectionMatrix = projectionMatrix;
		this->cutOff = cutOff;
		this->outerCutOff = outerCutOff;
	}


	SpotLight::~SpotLight()
	{
	}

	Camera* SpotLight::GetCamera() const
	{
		return this->camera;
	}

	float SpotLight::GetCutOff() const
	{
		return this->cutOff;
	}

	float SpotLight::GetOuterCutOff() const
	{
		return this->outerCutOff;
	}

	bool SpotLight::IsShadowCasting() const
	{
		return this->shadowCasting;
	}

	void SpotLight::Init()
	{
		this->GetEngine()->GetRenderPass()->AddSpotLight(this);
		this->GetEngine()->GetRenderPass()->DirtyLights(); // render pass needs to set up light stuff again
	}

	void SpotLight::Wire()
	{
	}

	void SpotLight::AttachedToEntity()
	{
		BaseLight::AttachedToEntity();

		// SpotLight needs a camera => create it, and wire it up
		this->camera = new Camera(projectionMatrix);
		this->GetEntity()->Add(camera);

		camera->EnableRender2Texture(this->shadowMapSize, this->shadowMapSize);
		camera->SetCameraPass(GetEngine()->GetDepthPass());
	}
}
