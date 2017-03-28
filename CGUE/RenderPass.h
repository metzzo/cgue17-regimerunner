#pragma once
#include "Pass.h"

namespace Engine {
	class Shader;
	class RenderPass : public Pass
	{
		Shader* shader;
		GLint shaderViewId;
		GLint shaderProjectionId;
		GLint shaderModelId;
		GLint shaderLightPosId;
		GLint shaderViewPosId;
		GLint shaderLightSpaceMatrixId;
		GLint shaderDiffuseTexture;
		GLint shaderShadowMap;
	public:
		explicit RenderPass(GameEngine *gameEngine);
		~RenderPass();

		void BeforePass() override;
		void AfterPass() override;
		void Init() override;
		void SetDrawingTransform(Transformation* transformation) const;
	};
}