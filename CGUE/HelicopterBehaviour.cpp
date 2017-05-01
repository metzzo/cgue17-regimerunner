#include "HelicopterBehaviour.h"
#include "GameEngine.h"
#include "Pass.h"
#include "Transformation.h"

namespace Game {
	void HelicopterBehaviourOperation::ChangeDirection()
	{
		duration = rand() % 200;
		direction = rand() % 40 - 20;
	}

	void HelicopterBehaviourOperation::Execute()
	{
		GetComponent()->GetTransformation()->Translate(vec3(0, 0, 3 + 0.05f*abs(direction)));
		GetComponent()->GetTransformation()->Rotate(direction*0.01f, vec3(0, 1, 0));

		duration--;

		if (duration < 0)
		{
			ChangeDirection();
		}
	}

	void HelicopterBehaviour::Init()
	{
		GetEngine()->GetUpdatePass()->AddOperation(new HelicopterBehaviourOperation(this));
	}
}
