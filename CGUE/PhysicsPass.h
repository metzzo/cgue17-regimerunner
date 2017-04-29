#pragma once
#include "Pass.h"

namespace Engine {
	class PhysicsPass :
		public Pass
	{
	public:
		explicit PhysicsPass(GameEngine *gameEngine);
		~PhysicsPass();

		void BeforePass() override;
		void AfterPass() override;
		void Init() override;
	};
}
