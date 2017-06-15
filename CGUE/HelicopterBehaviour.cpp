#include "HelicopterBehaviour.h"
#include "GameEngine.h"
#include "Pass.h"
#include "Transformation.h"
#include "Entity.h"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "HeightMapResource.h"
#include <SDL.h>
#include "Player.h"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/norm.hpp"
#include "glm/gtx/matrix_decompose.inl"
#include <iostream>
#include "Frustum.h"
#include "RigidBody.h"

using namespace glm;

namespace Game {
	void HelicopterBehaviourOperation::ChangeDirection() const
	{

		auto component = static_cast<HelicopterBehaviour*>(this->GetComponent());
		component->state = HelicopterMovementState(rand() % 5);
		component->start = SDL_GetTicks();
		component->duration = 2000 + rand() % 5000;
	}

	void Game::HelicopterBehaviourOperation::Execute()
	{
		// TODO: make frame independent

		auto component = static_cast<HelicopterBehaviour*>(this->GetComponent());
		auto dt = component->GetEngine()->GetDeltaTime();

		auto speed = 10.0f;
		if (!component->broken) {

			auto pos = component->GetTransformation()->GetAbsolutePosition();
			auto cumHeight = 0.0;
			for (auto x = -16; x < 16; x += 4)
			{
				for (auto z = -16; z < 16; z += 4)
				{
					auto height = component->heightMap->GetHeightAt(pos.x + x, pos.z + z);
					if (height == -1 && component->state != HMS_FOLLOW_PLAYER)
					{
						component->state = HMS_GOTO_CENTER;
					}
					cumHeight = max(cumHeight, height);
				}
			}
			auto translation = vec3(0, float(cumHeight) - pos.y + 100, 0);
			//cout << component->state << endl;
			switch(component->state)
			{
			case HMS_GOTO_CENTER:
			case HMS_FOLLOW_PLAYER: {
				glm::vec3 scale;
				glm::quat rotation;
				glm::vec3 IGNORE;
				glm::vec3 skew;
				glm::vec4 perspective;
				glm::decompose(component->GetTransformation()->GetRelativeMatrix(), scale, rotation, IGNORE, skew, perspective);

				auto targetPos = component->state == HMS_FOLLOW_PLAYER ? component->player->GetTransformation()->GetAbsolutePosition() : component->heightMap->GetSize()/2.0f;

				auto direction = targetPos - pos;
				direction.y = 0;

				if (length(direction) > 10) {
					auto angle = atan2(direction.z, direction.x);
					//auto rotMatrix = toMat4(slerp(rotation, angleAxis(-angle + radians(90.0f), vec3(0, 1, 0)), 0.005f*dt));
					auto rotMatrix = toMat4(angleAxis(-angle + radians(90.0f), vec3(0, 1, 0)));
					component->GetTransformation()->SetRelativeMatrix(translate(mat4(), pos)*rotMatrix);

					translation += vec3(0, 0, 1) * dt * 0.05f;
				} else if (component->state == HMS_GOTO_CENTER)
				{
					ChangeDirection();
				}

				break;
			}
			case HMS_LEFT_FORWARD:
			case HMS_RIGHT_FORWARD:
				component->GetTransformation()->Rotate(dt*0.05, vec3(0, component->state == HMS_LEFT_FORWARD ? 1 : -1, 0));
			case HMS_FORWARD:
				translation += vec3(0, 0, 0.025*dt);
				break;
			default:
				break;
			}

			component->GetTransformation()->Translate(translation);


			component->viewCamera->SetLookAtVector(component->spotLight->GetCamera()->GetLookAtVector());
			component->viewCamera->RefreshFrustum();

			if (SDL_GetTicks() - component->start > component->duration)
			{
				ChangeDirection();
			}

			auto spotLightPos = component->GetTransformation()->GetAbsolutePosition();
			spotLightPos.y = 0;
			auto spotLightDir = normalize(vec3(0, -1, 0.001)); //; vec3(cos(radians(component->spotLightTimer)) / 1.5f, -1, sin(radians(component->spotLightTimer)) / 1.5f);
			component->spotLight->GetCamera()->SetLookAtVector(spotLightPos + spotLightDir);

			// check if heli sees player
			if (component->viewCamera->PointInFrustum(component->player->GetTransformation()->GetAbsolutePosition()) != Engine::F_OUTSIDE)
			{
				component->spotLightTimer += dt * 0.00001;
				auto physicsPos = PxVec3(pos.x, pos.y, pos.z);
				auto raycastDir = normalize(component->player->GetTransformation()->GetAbsolutePosition() - pos);
				auto physicsDir = PxVec3(raycastDir.x, raycastDir.y, raycastDir.z);
				auto maxDistance = component->viewCamera->GetFar();
				PxRaycastBuffer hit;

				auto status = component->GetEngine()->GetPhysicsScene()->raycast(physicsPos, physicsDir, maxDistance, hit);
				if (status)
				{
					auto b = hit.block.actor;
					if (b->userData == nullptr) { // if it doesnt hit anything with a userdata => player is visible
						component->player->HelicopterHit();
						
						/*auto rigidBody = static_cast<Engine::RigidBody*>(b->userData);
						Player *player = nullptr;
						rigidBody->GetEntity()->WireUp(reinterpret_cast<Engine::Component**>(&player), PlayerClass.GetClassName(), false);
						if (player != nullptr) {*/
							//cout << "Helicopter sees you" << endl;
						//}
					}
				}
			}
			else
			{
				component->spotLightTimer = 0; // dt * 0.00001;

				//cout << "Helicopter does not see you" << endl;
			}

			speed = 100.0f;
		}
		component->mainRotor->Rotate(radians(speed * dt), vec3(0.0f, 1.0f, 0.0f));
		component->sideRotor->Rotate(radians(speed * dt), vec3(1.0f, 0.0f, 0.0f));

	}

	HelicopterBehaviour::HelicopterBehaviour(
		Engine::Transformation *mainRotor, 
		Engine::Transformation *sideRotor, 
		bool broken, 
		Engine::HeightMapResource *heightMap, 
		Player *player,
		Engine::Camera *viewCamera)
	{
		this->spotLight = nullptr;
		this->broken = broken;
		this->mainRotor = mainRotor;
		this->sideRotor = sideRotor;
		this->heightMap = heightMap;
		this->viewCamera = viewCamera;
		this->state = HMS_FOLLOW_PLAYER;
		this->player = player;
		this->start = 0;
		this->duration = 0;
		this->spotLightTimer = 0;
	}

	void HelicopterBehaviour::Init()
	{
		GetEngine()->GetUpdatePass()->AddOperation(new HelicopterBehaviourOperation(this));
	}

	void HelicopterBehaviour::Wire()
	{
		WIRE_COMPONENT(this->spotLight, Engine::SpotLightClass);
	}
}
