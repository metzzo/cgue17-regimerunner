#include "SpotLight.h"
#include "Camera.h"
#include "Entity.h"

namespace Engine {
	void LightRenderOperation::Execute()
	{
		auto component = static_cast<SpotLight*>(this->GetComponent());

		// render depth buffer
		glBindFramebuffer(GL_FRAMEBUFFER, component->depthMapFbo);
		glClear(GL_DEPTH_BUFFER_BIT);

		component->camera->RenderScreen(DEPTH_PASS_OPERATION);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	OPERATION_TYPE LightRenderOperation::GetOperationType()
	{
		return LIGHT_PASS_OPERATION;
	}

	GLuint LightRenderOperation::GetDepthTexture() const
	{
		auto component = static_cast<SpotLight*>(this->GetComponent());
		return component->depthMap;
	}

	SpotLight::SpotLight(int shadowMapSize, float near, float far)
	{
		this->shadowMapSize = shadowMapSize;
		this->camera = nullptr;
		this->depthMapFbo = 0;
		this->depthMap = 0;
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
		// create depth map FBO
		glGenFramebuffers(1, &this->depthMapFbo);
		glGenTextures(1, &this->depthMap);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
			this->shadowMapSize, this->shadowMapSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


		GetEngine()->AddOperation(new LightRenderOperation(this));
	}

	void SpotLight::Wire()
	{
		WIRE_COMPONENT(this->camera, CameraClass);
	}

	void SpotLight::AttachedToEntity()
	{
		// SpotLight needs a camera => create it, and wire it up
		auto camera = new Camera(0, near, far, shadowMapSize, shadowMapSize);
		this->GetEntity()->Add(camera);
	}
}
