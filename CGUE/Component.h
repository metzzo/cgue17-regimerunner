#pragma once
#include "EntityUnit.h"

namespace Engine {
	class Component : public EntityUnit
	{
	public:
		Component();
		virtual ~Component();

		virtual void Update();
	};
}

