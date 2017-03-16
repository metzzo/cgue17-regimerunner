#include "SpotLight.h"
#include "Camera.h"
#include "Entity.h"

namespace Engine {
	SpotLight::SpotLight(int shadowMapSize, float near, float far)
	{
		this->shadowMapSize = shadowMapSize;
		this->camera = nullptr;
		this->near = near;
		this->far = far;
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
		this->camera = new Camera(0, near, far, shadowMapSize, shadowMapSize, true);
		this->GetEntity()->Add(camera);

		camera->EnableRender2Texture(this->shadowMapSize, this->shadowMapSize, DEPTH_PASS_OPERATION);
	}
}
