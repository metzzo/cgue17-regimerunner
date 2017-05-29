#include "DirectionalLight.h"
#include "Entity.h"
#include "GameEngine.h"
#include "RenderPass.h"
#include "DepthPass.h"

namespace Engine {
	DirectionalLight::DirectionalLight(mat4 projectionMatrix, int shadowMapSize)
	{

		this->shadowMapSize = shadowMapSize;
		this->camera = nullptr;
		this->projectionMatrix = projectionMatrix;
	}


	DirectionalLight::~DirectionalLight()
	{
	}

	Camera* DirectionalLight::GetCamera() const
	{
		return this->camera;
	}

	void DirectionalLight::Init()
	{
		this->GetEngine()->GetRenderPass()->SetDirectionalLight(this);
	}

	void DirectionalLight::AttachedToEntity()
	{

		BaseLight::AttachedToEntity();

		// SpotLight needs a camera => create it, and wire it up
		this->camera = new Camera(projectionMatrix);
		this->GetEntity()->Add(camera);

		camera->EnableRender2Texture(this->shadowMapSize, this->shadowMapSize);
		camera->SetCameraPass(GetEngine()->GetDepthPass());
	}
}
