#include "CollectingPlaceBehaviour.h"
#include "GameEngine.h"
#include "Transformation.h"
#include "Pass.h"

namespace Game {
	void CollectingPlaceOperation::Execute()
	{
		auto component = static_cast<CollectingPlaceBehaviour*>(this->GetComponent());
		auto dt = component->GetEngine()->GetDeltaTime();

		component->GetTransformation()->Rotate(dt*0.1f, vec3(0, 1, 0));
		
	}

	CollectingPlaceBehaviour::CollectingPlaceBehaviour(Player* player)
	{
		this->player = player;
	}

	CollectingPlaceBehaviour::~CollectingPlaceBehaviour()
	{
	}

	void CollectingPlaceBehaviour::Init()
	{
		GetEngine()->GetUpdatePass()->AddOperation(new CollectingPlaceOperation(this));
	}
}
