#include "SpotLight.h"
#include "Camera.h"
#include "Entity.h"

namespace Engine {
	void SpotLightRenderOperation::Execute()
	{
		auto component = static_cast<SpotLight*>(this->GetComponent());

		// render depth buffer
		glBindFramebuffer(GL_FRAMEBUFFER, component->depthMapFbo);
		glClear(GL_DEPTH_BUFFER_BIT);

		component->camera->RenderScreen(QUEUE_DEPTH_PASS);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	QUEUE_TYPE SpotLightRenderOperation::GetQueueType()
	{
		return QUEUE_LIGHT_PASS;
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

	void SpotLight::Init()
	{
		// SpotLight needs a camera => create it, and wire it up
		auto camera = new Camera(0, near, far, shadowMapSize, shadowMapSize);
		this->GetEntity()->Add(camera);

		WIRE_COMPONENT(this->camera, CameraClass);

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
	}

	void SpotLight::Wire()
	{
	}
}
