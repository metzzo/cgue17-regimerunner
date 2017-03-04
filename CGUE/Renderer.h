#pragma once
#include "EntityUnit.h"

namespace Engine {
	class Entity;
	class GameEngine;

	class Renderer : public EntityUnit
	{
	public:
		Renderer();
		virtual ~Renderer();

		virtual void Render() = 0;
	};
}
