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

	public:
		Entity();
		virtual ~Entity();

		Component *AddComponent(Component *component);
		Renderer *AddRenderer(Renderer *renderer);

		void WireUp(Component **target, const char *name);
		void WireUp(Renderer **target, const char *name);

		void Update(GameEngine *gameEngine);
		void Render(GameEngine *gameEngine);
		void Init(GameEngine *gameEngine);
		void Wire();
	};
}
