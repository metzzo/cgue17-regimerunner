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
#include "RenderPass.h"
#include "Player.h"

namespace Game {
	void CameraMovementOperation::Execute() {
		auto component = static_cast<CameraMovement*>(this->GetComponent());
		auto engine = this->GetComponent()->GetEngine();
		auto xspeed = engine->GetMouseXRel();
		auto yspeed = engine->GetMouseYRel();
		auto cam = component->camera;
		auto keyDown = engine->KeyDown(SDL_SCANCODE_S);
		auto keyUp = engine->KeyDown(SDL_SCANCODE_W);
		auto keyLeft = engine->KeyDown(SDL_SCANCODE_A);
		auto keyRight = engine->KeyDown(SDL_SCANCODE_D);
		auto keySpace = engine->KeyDown(SDL_SCANCODE_Q);
		auto camerapos = cam->GetTransformation()->GetAbsolutePosition();

		// key
		auto physicsPos = component->controller->getPosition();
		auto pos = vec3(physicsPos.x, physicsPos.y, physicsPos.z);

		if (pos != camerapos) {
			//cout << pos.x << " " << pos.y << " " << pos.z << endl;
			component->GetTransformation()->SetRelativeMatrix(translate(mat4(), pos));
		}

		yaw += xspeed;
		pitch -= yspeed;

		if (pitch > 89.0f)
		{
			pitch = 89.0f;
		}
		if (pitch < -89.0f)
		{
			pitch = -89.0f;
		}

		glm::vec3 front;
		front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		front.y = sin(glm::radians(pitch));
		front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
		auto cameraFront = glm::normalize(front);

		front.x = cos(glm::radians(-pitch)) * cos(glm::radians(yaw));
		front.y = sin(glm::radians(-pitch));
		front.z = cos(glm::radians(-pitch)) * sin(glm::radians(yaw));
		auto waterCameraFront = glm::normalize(front);

		component->spotLight->GetCamera()->SetLookAtVector(component->spotLight->GetTransformation()->GetAbsolutePosition() + cameraFront);
		cam->SetLookAtVector(pos + cameraFront);
		component->refractionCamera->SetLookAtVector(pos + cameraFront);

		component->reflectionCamera->GetTransformation()->SetRelativeMatrix(translate(mat4(), vec3(0, -2 * (pos.y - 20), 0)));
		component->reflectionCamera->SetLookAtVector(component->reflectionCamera->GetTransformation()->GetAbsolutePosition() + waterCameraFront);


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
		desc.height = player->GetHeight();
		desc.radius = player->GetRadius();
		desc.userData = GetEntity();
		desc.position = PxExtendedVec3(pos.x, pos.y, pos.z);
		desc.material = GetEngine()->GetPhysics()->createMaterial(0.25f, 0.25f, 0.25f);

		this->controller = GetEngine()->GetControllerManager()->createController(desc);
		assert(this->controller != nullptr);

		GetEngine()->GetUpdatePass()->AddOperation(new CameraMovementOperation(this));
	}

	CameraMovement::CameraMovement(Engine::SpotLight* spotLight, Engine::Camera* reflectionCamera, Engine::Camera *refractionCamera, Player *player) : Component()
	{
		this->spotLight = spotLight;
		this->controller = nullptr;
		this->camera = nullptr;
		this->reflectionCamera = reflectionCamera;
		this->refractionCamera = refractionCamera;
		this->player = player;
	}

	void CameraMovement::Wire()
	{
		WIRE_COMPONENT(this->camera, Engine::CameraClass);
	}
}
