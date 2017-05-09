#pragma once
#include "Pass.h"
#include "glew/glew.h"

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
		GLint lightSpotdir;
		GLint lightSpotCutoff;
		GLint lightSpotOuterCutoff;
	public:
		explicit RenderPass(GameEngine *gameEngine);
		~RenderPass();

		void BeforePass() override;
		void AfterPass() override;
		void Init() override;
		void SetDrawingTransform(Transformation* transformation) const;
		GLint GetDiffuseUniform(int number) const;
		GLint GetSpecularUniform(int number) const;
	};
}
