#pragma once
namespace Engine {
	class GameEngine;
	class Entity;

	class Component
	{
		friend Entity;

		Entity *entity;

	public:
		Component();
		virtual ~Component();

		Entity *getEntity() const;
		virtual void Update(GameEngine *gameEngine);
		virtual void Init(GameEngine* gameEngine);
		virtual void Wire();
	};
}

