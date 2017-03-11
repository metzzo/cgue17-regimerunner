#include "Camera.h"
#include "Entity.h"
#include "GameEngine.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

namespace Engine {
	Camera::Camera()
	{
		// Do NOT use this constructor directly
		this->fov = 0;
		this->near = 0;
		this->far = 0;
	}


	Camera::Camera(float fov, float near, float far)
	{
		this->fov = fov;
		this->near = near;
		this->far = far;
	}

	Camera::~Camera()
	{
	}

	mat4x4 Camera::GetProjectionViewMatrix() const
	{
		return this->projectionMatrix * this->GetEntity()->GetTransformation()->GetAbsoluteMatrix();
	}

	void Camera::UpdateProjectionView() const
	{
		this->GetEngine()->GetRootEntity()->GetTransformation()->SetProjectionViewMatrix(this->GetProjectionViewMatrix());
	}

	void Camera::Wire()
	{
	}

	void Camera::Init()
	{
		auto engine = this->GetEntity()->GetEngine();
		auto ratio = float(engine->GetScreenWidth()) / float(engine->GetScreenHeight());
		projectionMatrix = perspective(radians(fov), ratio, near, far);
	}
}
