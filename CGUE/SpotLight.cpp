#include "SpotLight.h"
#include "Entity.h"
#include "GameEngine.h"
#include "RenderPass.h"
#include "DepthPass.h"
#include "Model.h"
#include "TextureResource.h"
#include "SpriteResource.h"

namespace Engine {
	const SpotLight SpotLightClass;

	SpotLight::SpotLight()
	{
		
	}

	SpotLight::SpotLight(float cutOff, float outerCutOff)
	{
		this->cutOff = cutOff;
		this->outerCutOff = outerCutOff;
		this->shadowCasting = false;
		this->camera = nullptr;
		this->shadowMapSize = -1;
	}

	SpotLight::SpotLight(mat4 projectionMatrix, int shadowMapSize, float cutOff, float outerCutoff)
	{
		this->cutOff = cutOff;
		this->outerCutOff = outerCutOff;
		this->shadowCasting = true;
		this->projectionMatrix = projectionMatrix;
		this->shadowMapSize = shadowMapSize;
		this->camera = nullptr;
	}

	SpotLight::~SpotLight()
	{
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

	bool SpotLight::IsShadowCasting() const
	{
		return shadowCasting;
	}

	void SpotLight::AttachedToEntity()
	{

		BaseLight::AttachedToEntity();

		// SpotLight needs a camera => create it, and wire it up
		this->camera = new Camera(projectionMatrix, this->shadowMapSize, this->shadowMapSize);
		this->GetEntity()->Add(camera);

		if (this->shadowCasting) {
			camera->EnableRender2Texture();
			camera->SetCameraPass(GetEngine()->GetDepthPass());


			auto hudTest = GetEngine()->GetRootEntity()->CreateChild();
			hudTest->GetTransformation()->Scale(vec3(0.25, 0.25, 1));
			auto spriteResource = new SpriteResource(camera);
			hudTest->Add(new Model(spriteResource));
		} else
		{
			this->camera->RenderingEnabled(false);
		}
	}

	Camera* SpotLight::GetCamera() const
	{
		return this->camera;
	}
}
