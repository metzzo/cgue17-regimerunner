#include "CollectingPlaceBehaviour.h"
#include "GameEngine.h"
#include "Transformation.h"
#include "Pass.h"
#include "Player.h"
#include "WoodInteraction.h"
#include <SDL.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.inl"

namespace Game {
	void CollectingPlaceOperation::Execute()
	{
		auto component = static_cast<CollectingPlaceBehaviour*>(this->GetComponent());
		auto selectButton = component->GetEngine()->KeyDownLastFrame(SDL_SCANCODE_E);
		auto dt = component->GetEngine()->GetDeltaTime();

		component->GetTransformation()->Rotate(dt*0.1f, vec3(0, 1, 0));


		if (selectButton && component->player->GetWood() != nullptr)
		{
			auto posWood = component->GetTransformation()->GetAbsolutePosition();
			auto cam = component->GetEngine()->GetMainCamera();
			auto posCamera = cam->GetTransformation()->GetAbsolutePosition();
			auto dist = glm::distance(posWood, posCamera);

			if (dist < 120.0) {
				auto wood = component->player->GetWood();

				wood->Place(component);
			}
		}
	}

	int CollectingPlaceOperation::GetPriority()
	{
		return -1000;
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
