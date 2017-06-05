#include "HelicopterBehaviour.h"
#include "GameEngine.h"
#include "Pass.h"
#include "Transformation.h"
#include "Entity.h"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

namespace Game {
	void HelicopterBehaviourOperation::ChangeDirection()
	{
		duration = rand() % 200;
		direction = rand() % 40 - 20;
	}

	void HelicopterBehaviourOperation::Execute()
	{
		// TODO: make frame independent

		auto component = static_cast<HelicopterBehaviour*>(this->GetComponent());
		auto dt = component->GetEngine()->GetDeltaTime()*0.1f;
		
		if (!component->broken) {
			component->GetTransformation()->Translate(vec3(0, 0, 3 + 0.05f*abs(direction))*dt);
			component->GetTransformation()->Rotate(direction*0.01f*dt, vec3(0, 1, 0));
			component->spotLight->GetCamera()->SetLookAtVector(component->GetTransformation()->GetAbsolutePosition() + vec3(0, -1, 0.001));
		} else
		{
			component->mainRotor->Rotate(radians(10.0f * dt), vec3(0.0f, 1.0f, 0.0f));
			component->sideRotor->Rotate(radians(10.0f * dt), vec3(0.0f, 1.0f, 0.0f));
		}


		duration -= 0.01f*dt;

		if (duration < 0)
		{
			ChangeDirection();
		}
	}

	HelicopterBehaviour::HelicopterBehaviour(Engine::Transformation *mainRotor, Engine::Transformation *sideRotor, bool broken)
	{
		this->spotLight = nullptr;
		this->broken = broken;
		this->mainRotor = mainRotor;
		this->sideRotor = sideRotor;
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
