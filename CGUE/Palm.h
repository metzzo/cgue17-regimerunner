#pragma once
#include "Entity.h"
#include "ModelResource.h"
#include "Model.h"
#include "GameEngine.h"

using namespace Engine;

namespace Game {

	class Palm : public Engine::Entity
	{
		ModelResource *m;
		
	public:
		Palm(GameEngine *engine, ModelResource *res) : Entity(engine) {
			this->Add(new Model(res));
		};

		void PlaceRandom(int height, int width);

	};
}