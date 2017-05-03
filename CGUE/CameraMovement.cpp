#include "CameraMovement.h"
#include "Pass.h"
#include "Transformation.h"
#include "glm/gtx/transform.hpp"
#include <SDL.h>
#include "Camera.h"
#include <iostream>
#include "glm/gtx/matrix_decompose.hpp"
#include "glm/gtx/quaternion.hpp"
#include "GameEngine.h"
#include "Entity.h"
#include "Camera.h"

namespace Game {

	void CameraMovementMouseOperation::Execute()
	{
		auto component = static_cast<CameraMovement*>(GetComponent());
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
		component->camera->SetLookAtVector(camerapos + cameraFront);
	}

	void CameraMovementKeyOperation::Execute() {
		auto component = static_cast<CameraMovement*>(this->GetComponent());
		auto cam = component->GetEngine()->GetMainCamera();

		auto cameraFront = cam->GetLookAtVector() - cam->GetTransformation()->GetAbsolutePosition();

		auto physicsPos = component->controller->getPosition();
		auto pos = vec3(physicsPos.x, physicsPos.y, physicsPos.z) ;
		component->GetTransformation()->SetRelativeMatrix(translate(mat4(), pos));
		cam->SetLookAtVector(pos + cameraFront);
		component->spotLight->GetCamera()->SetLookAtVector(pos + cameraFront);

		auto keyDown = component->GetEngine()->KeyDown(SDL_SCANCODE_S);
		auto keyUp = component->GetEngine()->KeyDown(SDL_SCANCODE_W);
		auto keyLeft = component->GetEngine()->KeyDown(SDL_SCANCODE_A);
		auto keyRight = component->GetEngine()->KeyDown(SDL_SCANCODE_D);
		auto keySpace = component->GetEngine()->KeyDown(SDL_SCANCODE_Q);

		auto direction = vec3(0.0f, -9.81f, 0.0f) + jump;
		if (keyDown || keyUp || keyLeft || keyRight) {
			auto cameraSpeed = 1.0f;

			direction += cameraSpeed * (
				cameraFront * static_cast<float>(keyUp - keyDown) +
				glm::normalize(glm::cross(cameraFront, cam->GetUpVector())) * static_cast<float>(keyRight - keyLeft)
			);
		}

		if (keySpace && !jumpPress)
		{
			jumpPress = true;
 			jump = vec3(0.0f, 9.81f, 0.0f)*1.5f;
		} else if (!keySpace && jumpPress)
		{
			jumpPress = false;
		}

		jump *= 0.95;


		component->controller->move(PxVec3(direction.x, direction.y, direction.z), 0.1f, 1, nullptr);
	}


	void CameraMovement::Init()
	{	
		auto pos = GetTransformation()->GetAbsolutePosition();

		PxCapsuleControllerDesc desc;
		desc.height = 8.0;
		desc.radius = 1.0;
		desc.userData = GetEntity();
		desc.position = PxExtendedVec3(pos.x, pos.y, pos.z);
		desc.material = GetEngine()->GetPhysics()->createMaterial(0.25f, 0.25f, 0.25f);

		this->controller = GetEngine()->GetControllerManager()->createController(desc);
		assert(this->controller != nullptr);

		GetEngine()->GetUpdatePass()->AddOperation(new CameraMovementMouseOperation(this));
		GetEngine()->GetUpdatePass()->AddOperation(new CameraMovementKeyOperation(this));
	}

	CameraMovement::CameraMovement(Engine::SpotLight* spotLight) : Component()
	{
		this->spotLight = spotLight;
	}

	void CameraMovement::Wire()
	{
		WIRE_COMPONENT(this->camera, Engine::CameraClass);
	}
}
