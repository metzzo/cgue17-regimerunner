#pragma once
#include <extensions/PxFixedJoint.h>

namespace Engine {
	class Entity;
	class Transformation;
	class Component;
	class Renderer;
	class GameEngine;

	class Component
	{
		friend Entity;

		Entity *entity;
	public:
		Component();
		virtual ~Component();

		Entity* GetEntity() const;
		GameEngine* GetEngine() const;
		Transformation* GetTransformation() const;

		virtual void TransformationUpdated();
		virtual void AttachedToEntity();
		virtual void Init();
		virtual void Wire();
		const char *GetClassName() const;
	};
}

