#include "CameraMovement.h"
#include "Pass.h"
#include "Transformation.h"
#include "glm/gtx/transform.hpp"
#include <SDL.h>
#include "Camera.h"
#include <iostream>
#include "glm/gtx/matrix_decompose.hpp"
#include "glm/gtx/quaternion.hpp"

namespace Game {

	void CameraMovementMouseOperation::Execute()
	{
		auto engine = this->GetComponent()->GetEngine();

		auto cam = engine->GetMainCamera();
		auto xspeed = engine->GetMouseXRel();
		auto yspeed = engine->GetMouseYRel();
		auto camerapos = cam->GetTransformation()->GetAbsolutePosition();

		yaw += xspeed;
		pitch -= yspeed;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		front.y = sin(glm::radians(pitch));
		front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
		auto cameraFront = glm::normalize(front);

		cam->SetLookAtVector(camerapos + cameraFront);
	}

	void CameraMovementKeyOperation::Execute() {

		auto component = this->GetComponent();
		auto cam = component->GetEngine()->GetMainCamera();

		auto keyDown = component->GetEngine()->KeyDown(SDL_SCANCODE_DOWN);
		auto keyUp = component->GetEngine()->KeyDown(SDL_SCANCODE_UP);
		auto keyLeft = component->GetEngine()->KeyDown(SDL_SCANCODE_LEFT);
		auto keyRight = component->GetEngine()->KeyDown(SDL_SCANCODE_RIGHT);

		if (keyDown || keyUp || keyLeft || keyRight) {
			auto camerapos = cam->GetTransformation()->GetAbsolutePosition();
			auto cameraFront = cam->GetLookAtVector() - cam->GetTransformation()->GetAbsolutePosition();
			auto cameraSpeed = 1.0f;

			camerapos += cameraSpeed * (
				cameraFront * static_cast<float>(keyUp - keyDown) +
				glm::normalize(glm::cross(cameraFront, cam->GetUpVector())) * static_cast<float>(keyRight - keyLeft)
			);

			cam->GetTransformation()->SetRelativeMatrix(translate(mat4(), camerapos));
			cam->SetLookAtVector(camerapos + cameraFront);

		}
	}


	void CameraMovement::Init()
	{	
		GetEngine()->GetUpdatePass()->AddOperation(new CameraMovementMouseOperation(this));
		GetEngine()->GetUpdatePass()->AddOperation(new CameraMovementKeyOperation(this));
	}
}
