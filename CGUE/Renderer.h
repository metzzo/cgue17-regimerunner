#pragma once
namespace Engine {
	class Entity;
	class GameEngine;

	class Renderer
	{
		friend Entity;

		Entity *entity;
	public:
		Renderer();
		virtual ~Renderer();

		Entity *getEntity() const;

		virtual void Render(GameEngine *gameEngine) = 0;
		virtual void Init(GameEngine* gameEngine);
		virtual void Wire();
	};
}