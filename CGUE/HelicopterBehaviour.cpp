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
		auto dt = component->GetEngine()->GetDeltaTime();
		
		auto speed = 10.0f;
		if (!component->broken) {
			component->GetTransformation()->Translate(vec3(0, 0, 0.05*dt));
			component->GetTransformation()->Rotate(dt*0.05, vec3(0, 1, 0));
			component->spotLight->GetCamera()->SetLookAtVector(component->GetTransformation()->GetAbsolutePosition() + vec3(0, -1, 0.1));
			
			speed = 100.0f;
		}
		component->mainRotor->Rotate(radians(speed * dt), vec3(0.0f, 1.0f, 0.0f));
		component->sideRotor->Rotate(radians(speed * dt), vec3(1.0f, 0.0f, 0.0f));


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
