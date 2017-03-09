#pragma once
#include "GameEngine.h"

namespace Engine {
	class Entity;
	class Transformation;

	class EntityUnit
	{
		friend Entity;

		Entity *entity;
	public:
		EntityUnit();
		virtual ~EntityUnit();

		Entity* GetEntity() const;
		GameEngine* GetEngine() const;
		Transformation* GetTransformation() const;

		virtual void Init();
		virtual void Wire();
		const char *GetClassName() const;
	};
}

