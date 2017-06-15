#pragma once
#include "Pass.h"
#include "Camera.h"


namespace Engine {
	class Shader;

	class DepthPass : public Pass
	{
		Shader* shader;
		GLuint mvpUniform;
	public:
		explicit DepthPass(GameEngine *gameEngine);
		~DepthPass();

		void BeforePass() override;
		void AfterPass() override;
		void Init() override;
		void SetDrawingTransform(Transformation* transformation) const;
	};
}
