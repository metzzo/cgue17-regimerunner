#pragma once
#include "Pass.h"


namespace Engine {
	class Shader;

	class DepthPass : public Pass
	{
		Shader* shader;
	public:
		explicit DepthPass(GameEngine *gameEngine);
		~DepthPass();

		void BeforePass() override;
		void AfterPass() override;
		void Init() override;
		void SetDrawingTransform(Transformation* transformation);
	};
}
