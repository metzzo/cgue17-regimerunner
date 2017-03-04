#pragma once

#include "Component.h"
#include <vector>
#include "Renderer.h"

namespace Engine {
#define WIRE_COMPONENT(A,B) this->getEntity()->WireUp(reinterpret_cast<Component**>(&A), B.GetClassName());
#define WIRE_RENDERER(A,B) this->getEntity()->WireUp(reinterpret_cast<Renderer**>(&A), B.GetClassName());


	class GameEngine;
	class Entity
	{
		std::vector<Component*> components;
		std::vector<Renderer*> renderer;
		GameEngine *gameEngine;

	public:
		Entity(GameEngine *gameEngine);
		virtual ~Entity();

		Component *Add(Component *component);
		Renderer *Add(Renderer *renderer);

		void WireUp(Component **target, const char *name);
		void WireUp(Renderer **target, const char *name);

		void Update();
		void Render();
		void Init();
		void Wire();
	};
}
