#include "SpotLight.h"
#include "Entity.h"
#include "GameEngine.h"
#include "DepthPass.h"

namespace Engine {
	SpotLight::SpotLight(mat4 projectionMatrix, int shadowMapSize)
	{
		this->shadowMapSize = shadowMapSize;
		this->camera = nullptr;
		this->projectionMatrix = projectionMatrix;
	}


	SpotLight::~SpotLight()
	{
	}

	Camera* SpotLight::GetCamera() const
	{
		return this->camera;
	}

	void SpotLight::Init()
	{
		this->GetEngine()->AddLight(this);
	}

	void SpotLight::Wire()
	{
	}

	void SpotLight::AttachedToEntity()
	{
		// SpotLight needs a camera => create it, and wire it up
		this->camera = new Camera(projectionMatrix);
		this->GetEntity()->Add(camera);

		camera->EnableRender2Texture(this->shadowMapSize, this->shadowMapSize);
		camera->SetCameraPass(GetEngine()->GetDepthPass());
	}
}
