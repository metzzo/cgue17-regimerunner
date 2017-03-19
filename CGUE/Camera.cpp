#include "Camera.h"
#include "Entity.h"
#include "GameEngine.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glew/glew.h"
#include "Pass.h"
#include "RenderPass.h"

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

	Camera::Camera(float fov, float near, float far, int width, int height, bool ortho)
	{
		this->fov = fov;
		this->near = near;
		this->far = far;
		this->width = width;
		this->height = height;
		this->ortho = ortho;
		this->depthMapFbo = 0;
		this->depthMap = 0;
		this->textureWidth = 0;
		this->textureHeight = 0;
		this->cameraPass = nullptr;
		this->upVector = vec3(0.0, 1.0, 0.0);
		this->r2t = false;
	}

	Camera::~Camera()
	{
	}

	float Camera::GetFov() const
	{
		return this->fov;
	}

	float Camera::GetFar() const
	{
		return this->far;
	}

	float Camera::GetNear() const
	{
		return this->near;
	}

	void Camera::EnableRender2Texture(int textureWidth, int textureHeight)
	{
		this->textureWidth = textureWidth;
		this->textureHeight = textureHeight;
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

	mat4x4 Camera::GetViewMatrix() const
	{
		return this->viewMatrix;
	}

	mat4x4 Camera::GetProjectionMatrix() const
	{
		return this->projectionMatrix;
	}

	mat4x4 Camera::GetProjectionViewMatrix() const
	{
		return GetProjectionMatrix() * GetViewMatrix();
	}

	GLuint Camera::GetTexture() const
	{
		return this->depthMap;
	}

	void Camera::Wire()
	{
	}

	void Camera::Init()
	{
		if (ortho)
		{
			projectionMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, this->near, this->far);
		}
		else {
			auto engine = this->GetEntity()->GetEngine();
			auto ratio = float(engine->GetScreenWidth()) / float(engine->GetScreenHeight());
			projectionMatrix = perspective(radians(fov), ratio, near, far);
		}

		if (this->cameraPass == nullptr)
		{
			this->cameraPass = GetEngine()->GetRenderPass();
		}

		if (this->r2t)
		{
			// create depth map FBO
			glGenFramebuffers(1, &this->depthMapFbo);
			glGenTextures(1, &this->depthMap);
			glBindTexture(GL_TEXTURE_2D, depthMap);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
				textureWidth, textureHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glBindFramebuffer(GL_FRAMEBUFFER, this->depthMapFbo);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		GetEngine()->GetCameraPass()->AddOperation(new CameraRenderOperation(this));
	}

	void Camera::TransformationUpdated()
	{
		auto pos = GetTransformation()->GetAbsolutePosition();
		this->viewMatrix = lookAt(pos, pos + lookAtVector, upVector);
	}
}
