#pragma once
#include "Pass.h"
#include "glew/glew.h"
#include "RenderPass.h"

namespace Engine {

	class WaterPass : public Pass {

		Shader* shader;
		GLint shaderViewId;
		GLint shaderProjectionId;
		GLint shaderModelId;
		GLint shaderLightSpaceMatrixId;
		GLint shaderViewPosId;

	public:
		 explicit WaterPass(GameEngine *gameEngine);
		~WaterPass();

		void BeforePass() override;
		void AfterPass() override;
		void Init() override;
		void SetDrawingTransform(Transformation* transformation);

	};

}
