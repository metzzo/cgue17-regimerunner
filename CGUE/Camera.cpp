#include "Camera.h"
#include "Entity.h"
#include "GameEngine.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glew/glew.h"

namespace Engine {
	const Camera CameraClass;

	Camera::Camera()
	{
		// Do NOT use this constructor directly
		this->fov = 0;
		this->near = 0;
		this->far = 0;
		this->width = 0;
		this->height = 0;
		this->ortho = false;
	}


	Camera::Camera(float fov, float near, float far, int width, int height, bool ortho)
	{
		this->fov = fov;
		this->near = near;
		this->far = far;
		this->width = width;
		this->height = height;
		this->ortho = ortho;
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

	mat4x4 Camera::GetProjectionViewMatrix() const
	{
		return this->projectionMatrix * this->GetEntity()->GetTransformation()->GetAbsoluteMatrix();
	}

	void Camera::Wire()
	{
	}

	void Camera::Init()
	{
		auto engine = this->GetEntity()->GetEngine();
		if (ortho)
		{
			projectionMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, this->near, this->far);
		}
		else {
			auto ratio = float(engine->GetScreenWidth()) / float(engine->GetScreenHeight());
			projectionMatrix = perspective(radians(fov), ratio, near, far);
		}
	}

	void Camera::RenderScreen()
	{
		auto oldMainCamera = this->GetEngine()->GetMainCamera();
		this->GetEngine()->SetMainCamera(this);

		glViewport(0, 0, width, height);
		// TODO: only draw objects that could potentially visible for the camera
		this->GetEngine()->ProcessQueue(QUEUE_RENDER_PASS);

		this->GetEngine()->SetMainCamera(oldMainCamera);
	}
}
