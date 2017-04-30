#include "CameraMovement.h"
#include "Pass.h"
#include "Transformation.h"
#include "glm/gtx/transform.hpp"
#include <SDL.h>
#include "Camera.h"
#include <iostream>

namespace Game {

	void CameraMovementMouseOperation::Execute()
	{
		auto component = this->GetComponent();

		auto cam = component->GetEngine()->GetMainCamera();
		auto mat = cam->GetViewMatrix();

		auto lookAt = cam->GetLookAtVector();
		
		auto yaw = component->GetEngine()->GetYaw();
		auto pitch = component->GetEngine()->GetPitch();

		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		glm::vec3 cameraFront = glm::normalize(front);
		cam->SetLookAtVector(cameraFront);

		vec3 camerapos = cam->GetTransformation()->GetAbsolutePosition();

		auto view = glm::lookAt(camerapos, camerapos + cameraFront, cam->GetUpVector());
		
		cam->SetViewMatrix(view);
	}

	void CameraMovementKeyOperation::Execute() {

		auto component = this->GetComponent();
		auto cam = component->GetEngine()->GetMainCamera();
		auto mat = cam->GetViewMatrix();
		
		auto cameraFront = cam->GetLookAtVector();
		vec3 camerapos = cam->GetTransformation()->GetAbsolutePosition();

		GLfloat cameraSpeed = 1.0f;
		if (component->GetEngine()->KeyDown(SDL_SCANCODE_DOWN))
			camerapos -= cameraSpeed * cameraFront;
		if (component->GetEngine()->KeyDown(SDL_SCANCODE_UP))
			camerapos += cameraSpeed * cameraFront;
		if (component->GetEngine()->KeyDown(SDL_SCANCODE_LEFT))
			camerapos -= glm::normalize(glm::cross(cameraFront, cam->GetUpVector())) * cameraSpeed;
		if (component->GetEngine()->KeyDown(SDL_SCANCODE_RIGHT))
			camerapos += glm::normalize(glm::cross(cameraFront, cam->GetUpVector())) * cameraSpeed;

		auto view = glm::lookAt(camerapos, camerapos + cameraFront, cam->GetUpVector());
		
		cam->SetViewMatrix(view);
		

		/*
		auto cam = component->GetEngine()->GetMainCamera();
		auto mat = cam->GetViewMatrix();
		GLfloat cameraSpeed = 1.0f;
		if (component->GetEngine()->KeyDown(SDL_SCANCODE_DOWN))
			mat = glm::translate(mat, vec3(-cameraSpeed, 0.0f, 0.0f));
		if (component->GetEngine()->KeyDown(SDL_SCANCODE_UP))
			mat = glm::translate(mat, vec3(cameraSpeed, 0.0f, 0.0f));
		if (component->GetEngine()->KeyDown(SDL_SCANCODE_LEFT))
			mat = glm::translate(mat, vec3(0.0f, 0.0f, -cameraSpeed));
		if (component->GetEngine()->KeyDown(SDL_SCANCODE_RIGHT))
			mat = glm::translate(mat, vec3(0.0f, 0.0f, cameraSpeed));

		cam->SetViewMatrix(mat);
		*/
	}


	void CameraMovement::Init()
	{	
		GetEngine()->GetUpdatePass()->AddOperation(new CameraMovementMouseOperation(this));
		GetEngine()->GetUpdatePass()->AddOperation(new CameraMovementKeyOperation(this));
	}
}
