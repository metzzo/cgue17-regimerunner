#include "Camera.h"
#include "Entity.h"
#include "GameEngine.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glew/glew.h"
#include "Pass.h"
#include "RenderPass.h"
#include <SDL.h>

namespace Engine {
	const Camera CameraClass;

	void CameraRenderOperation::Execute()
	{
		auto component = static_cast<Camera*>(this->GetComponent());

		auto oldMainCamera = component->GetEngine()->GetMainCamera();
		component->GetEngine()->SetMainCamera(component);

		glViewport(0, 0, component->width, component->height);
		if (component->depthMapFbo)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, component->depthMapFbo);
		}
		// TODO: only draw objects that could potentially visible for the camera
		component->cameraPass->DoPass();

		if (component->depthMapFbo)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		

		component->GetEngine()->SetMainCamera(oldMainCamera);
	}

	int CameraRenderOperation::GetPriority()
	{
		if (GetComponent()->GetEngine()->GetMainCamera() == GetComponent())
		{
			return MAIN_CAMERA_PRIORITY; // main camera should have very high priority
		}
		// otherwise default
		return Operation::GetPriority();
	}

	Camera::Camera()
	{
		// dont use this constructor
	}

	Camera::Camera(mat4x4 projectionMatrix, int width, int height)
	{
		this->width = width;
		this->height = height;
		this->depthMapFbo = 0;
		this->depthMap = 0;
		this->cameraPass = nullptr;
		this->upVector = vec3(0.0, 1.0, 0.0);
		this->r2t = false;
		this->projectionMatrix = projectionMatrix;
		this->renderingEnabled = true;
	}

	Camera::~Camera()
	{
	}

	void Camera::RenderingEnabled(bool enabled)
	{
		this->renderingEnabled = enabled;
	}

	void Camera::EnableRender2Texture()
	{
		this->r2t = true;
	}

	void Camera::SetCameraPass(Pass* pass)
	{
		this->cameraPass = pass;
	}

	void Camera::SetLookAtVector(vec3 lookAt)
	{
		this->lookAtVector = lookAt;
		this->TransformationUpdated();
	}

	void Camera::SetUpVector(vec3 upVector)
	{
		this->upVector = upVector;
	}

	vec3 Camera::GetLookAtVector() const
	{
		return this->lookAtVector;
	}

	mat4x4 Camera::GetViewMatrix() const
	{
		return this->viewMatrix;
	}

	mat4x4 Camera::GetProjectionMatrix() const
	{
		return this->projectionMatrix;
	}

	mat4x4 Camera::GetHudProjectionMatrix() const
	{
		return this->hudProjectionMatrix;
	}

	GLuint Camera::GetTextureId()
	{
		return this->depthMap;
	}

	int Camera::GetWidth()
	{
		return this->width;
	}

	int Camera::GetHeight()
	{
		return this->height;
	}

	void Camera::Wire()
	{
	}

	void Camera::Init()
	{
		if (!renderingEnabled)
		{
			return;
		}

		if (this->cameraPass == nullptr)
		{
			this->cameraPass = GetEngine()->GetRenderPass();
		}
		DEBUG_OGL();

		if (this->r2t)
		{
			// create depth map FBO
			glGenFramebuffers(1, &this->depthMapFbo);
			glGenTextures(1, &this->depthMap);
			glBindTexture(GL_TEXTURE_2D, depthMap);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
				this->width, this->height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

			glBindFramebuffer(GL_FRAMEBUFFER, this->depthMapFbo);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		DEBUG_OGL();

		GetEngine()->GetCameraPass()->AddOperation(new CameraRenderOperation(this));
	}

	vec3 Camera::GetUpVector() const
	{
		return this->upVector;

	}

	void Camera::TransformationUpdated()
	{
		auto pos = GetTransformation()->GetAbsolutePosition();
		this->viewMatrix = lookAt(pos, lookAtVector, upVector);
	}

	void Camera::SetHudProjectionMatrix(mat4x4 hudMatrix)
	{
		this->hudProjectionMatrix = hudMatrix;
	}
}
