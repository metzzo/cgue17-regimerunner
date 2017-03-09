#pragma once

#include "Component.h"
#include <vector>
#include "Renderer.h"

namespace Engine {
#define WIRE_COMPONENT(A,B) this->GetEntity()->WireUp(reinterpret_cast<Component**>(&A), B.GetClassName());
#define WIRE_RENDERER(A,B) this->GetEntity()->WireUp(reinterpret_cast<Renderer**>(&A), B.GetClassName());


	class GameEngine;
	class Transformation;
	class Entity
	{
		vector<Component*> components;
		vector<Renderer*> renderer;
		vector<Entity*> children;
		Entity *parent;
		GameEngine *gameEngine;
		Transformation *transformation;
	public:
		explicit Entity(GameEngine *gameEngine);
		virtual ~Entity();

		GameEngine *GetEngine() const;

		Component *Add(Component *component);
		Renderer *Add(Renderer *renderer);
		Entity *CreateChild();

		Entity *GetParent() const;
		const vector<Entity*>* GetChildren() const;
		Transformation *GetTransformation() const;

		void WireUp(Component **target, const char *name);
		void WireUp(Renderer **target, const char *name);

		void Update();
		void Render();
		void Init();
		void Wire();
	};
}
