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

using namespace glm;

namespace Game {
	quat RotationBetweenVectors(vec3 start, vec3 dest) {
		start = normalize(start);
		dest = normalize(dest);

		float cosTheta = dot(start, dest);
		vec3 rotationAxis;

		if (cosTheta < -1 + 0.001f) {
			// special case when vectors in opposite directions:
			// there is no "ideal" rotation axis
			// So guess one; any will do as long as it's perpendicular to start
			rotationAxis = cross(vec3(0.0f, 0.0f, 1.0f), start);
			if (length2(rotationAxis) < 0.01) // bad luck, they were parallel, try again!
				rotationAxis = cross(vec3(1.0f, 0.0f, 0.0f), start);

			rotationAxis = normalize(rotationAxis);
			return angleAxis(180.0f, rotationAxis);
		}

		rotationAxis = cross(start, dest);

		float s = sqrt((1 + cosTheta) * 2);
		float invs = 1 / s;

		return quat(
			s * 0.5f,
			rotationAxis.x * invs,
			rotationAxis.y * invs,
			rotationAxis.z * invs
		);

	}

	void HelicopterBehaviourOperation::ChangeDirection()
	{

	}

	void HelicopterBehaviourOperation::Execute()
	{
		// TODO: make frame independent

		auto component = static_cast<HelicopterBehaviour*>(this->GetComponent());
		auto dt = component->GetEngine()->GetDeltaTime();

		auto speed = 10.0f;
		if (!component->broken) {
			auto pos = component->GetTransformation()->GetAbsolutePosition();
			auto targetHeight = component->heightMap->GetHeightAt(pos.x, pos.z) + 130;
			auto translation = vec3(0, (targetHeight - pos.y) / 100.0f * dt, 0);

			vec3 targetPos;
			vec3 dir;
			quat rot;

			switch(component->state)
			{
			case HMS_FOLLOW_PLAYER: {
				/*targetPos = component->player->GetTransformation()->GetAbsolutePosition();

				vec3 direction = targetPos - pos;
				if (direction.length() > 1) {
					direction = normalize(direction);
					quat rot1 = RotationBetweenVectors(vec3(0.0f, 0.0f, 1.0f), direction);
					vec3 right = cross(direction, vec3(0, 1, 0));
					vec3 desiredUp = cross(right, direction);

					// Because of the 1rst rotation, the up is probably completely screwed up.
					// Find the rotation between the "up" of the rotated object, and the desired up
					vec3 newUp = rot1 * vec3(0.0f, 1.0f, 0.0f);
					quat rot2 = RotationBetweenVectors(newUp, desiredUp);
					quat targetOrientation = rot2 * rot1;

					auto mat = toMat4(targetOrientation);
					mat = translate(mat, pos);

					component->GetTransformation()->SetRelativeMatrix(mat);
				}*/


				//translation += vec3(0,0,0.25 * dt * 0.05f);
			}
				break;
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


			component->spotLight->GetCamera()->SetLookAtVector(component->GetTransformation()->GetAbsolutePosition() + vec3(cos(SDL_GetTicks() / 1000.0f) / 1.5f, -1, sin(SDL_GetTicks()/1000.0f)/1.5f));
			
			speed = 100.0f;
		}
		component->mainRotor->Rotate(radians(speed * dt), vec3(0.0f, 1.0f, 0.0f));
		component->sideRotor->Rotate(radians(speed * dt), vec3(1.0f, 0.0f, 0.0f));

	}

	HelicopterBehaviour::HelicopterBehaviour(Engine::Transformation *mainRotor, Engine::Transformation *sideRotor, bool broken, Engine::HeightMapResource *heightMap, Player *player)
	{
		this->spotLight = nullptr;
		this->broken = broken;
		this->mainRotor = mainRotor;
		this->sideRotor = sideRotor;
		this->heightMap = heightMap;
		this->state = HMS_FOLLOW_PLAYER;
		this->player = player;
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
