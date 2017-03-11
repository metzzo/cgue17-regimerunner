#pragma once
#include "Component.h"

namespace Game {
	class Rotating : public Engine::Component
	{
	public:
		Rotating();
		~Rotating();

		void Update() override;
	};
}
