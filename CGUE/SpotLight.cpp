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
		this->fov = 0;
		this->near = 0;
		this->far = 0;
	}

	SpotLight::SpotLight(float fov, float near, float far, int shadowMapSize, float cutOff, float outerCutoff)
	{

		this->cutOff = cutOff;
		this->outerCutOff = outerCutoff;
		this->shadowCasting = true;
		this->shadowMapSize = shadowMapSize;
		this->camera = nullptr;
		this->fov = fov;
		this->near = near;
		this->far = far;
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
		this->camera = new Camera(fov, near, far, this->shadowMapSize, this->shadowMapSize);
		this->GetEntity()->Add(camera);

		if (this->shadowCasting) {
			//camera->SetUpVector(vec3(0, -1, 0));
			camera->SetCameraMode(CM_DEPTH);
			camera->SetCameraPass(GetEngine()->GetDepthPass());


			/*auto hudTest = GetEngine()->GetRootEntity()->CreateChild();
			hudTest->GetTransformation()->Scale(vec3(0.25, 0.25, 1));
			hudTest->GetTransformation()->Translate(vec3(300,300,0));
			auto spriteResource = new SpriteResource(camera);
			hudTest->Add(new Model(spriteResource));*/
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
