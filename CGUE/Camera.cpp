#include "Camera.h"
#include "Entity.h"
#include "GameEngine.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glew/glew.h"

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
		if (component->renderOperation == DEPTH_PASS_OPERATION) {
			glClear(GL_DEPTH_BUFFER_BIT);
		} else
		{
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		}
		// TODO: only draw objects that could potentially visible for the camera
		component->GetEngine()->ProcessQueue(component->renderOperation);
		if (component->depthMapFbo)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		component->GetEngine()->SetMainCamera(oldMainCamera);
	}

	OPERATION_TYPE CameraRenderOperation::GetOperationType()
	{
		return CAMERA_PASS_OPERATION;
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
		this->renderOperation = RENDER_PASS_OPERATION;
		this->upVector = vec3(0.0, 1.0, 0.0);
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

	void Camera::EnableRender2Texture(int textureWidth, int textureHeight, OPERATION_TYPE renderOperation)
	{
		this->renderOperation = renderOperation;
		this->textureWidth = textureWidth;
		this->textureHeight = textureHeight;
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

		if (this->renderOperation == DEPTH_PASS_OPERATION)
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

		GetEngine()->AddOperation(new CameraRenderOperation(this));
	}

	void Camera::TransformationUpdated()
	{
		auto pos = GetTransformation()->GetAbsolutePosition();
		this->viewMatrix = lookAt(pos, pos + lookAtVector, upVector);
	}
}
