#pragma once

#include "Component.h"
#include <vector>
#include "Renderer.h"

namespace Engine {
	class GameEngine;
	class Entity
	{
		std::vector<Component*> components;
		std::vector<Renderer*> renderer;

	public:
		Entity();
		virtual ~Entity();

		Component *AddComponent(Component *component);
		Renderer *AddRenderer(Renderer *renderer);

		void WireUp(Component **target, type_info *typeId);
		void WireUp(Renderer **target, type_info *typeId);

		void Update(GameEngine *gameEngine);
		void Render(GameEngine *gameEngine);
		void Init(GameEngine *gameEngine);
		void Wire();
	};
}
