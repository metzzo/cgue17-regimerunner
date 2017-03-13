#pragma once

#include "Component.h"
#include <vector>

namespace Engine {
#define WIRE_COMPONENT(A,B) this->GetEntity()->WireUp(reinterpret_cast<Component**>(&A), B.GetClassName());
#define WIRE_COMPONENT_OPTIONAL(A,B) this->GetEntity()->WireUp(reinterpret_cast<Component**>(&A), B.GetClassName(), true);

	class GameEngine;
	class Transformation;
	class Entity
	{
		vector<Component*> components;
		vector<Entity*> children;
		Entity *parent;
		GameEngine *gameEngine;
		Transformation *transformation;
	public:
		explicit Entity(GameEngine *gameEngine);
		virtual ~Entity();

		GameEngine *GetEngine() const;

		Component *Add(Component *component);
		Entity *CreateChild();

		Entity *GetParent() const;
		const vector<Entity*>* GetChildren() const;
		Transformation *GetTransformation() const;

		void WireUp(Component **target, const char *name, bool force=true);

		void Init();
		void Wire();
	};
}
