#pragma once
#include "Pass.h"
#include "glew/glew.h"

namespace Engine {
	class SpotLight;

	struct SpotLightInfo
	{
		GLint directionUniform;
		GLint positionUniform;
		GLint ambientUniform;
		GLint diffuseUniform;
		GLint specularUniform;
		GLint cutOffUniform;
		GLint outerCutOffUniform;
		GLint constantUniform;
		GLint linearUniform;
		GLint quadraticUniform;

		GLint isShadowUniform;
		GLint shadowMapUniform;
		GLint lightPosUniform;
	};

	class Shader;
	class RenderPass : public Pass
	{
		vector<SpotLightInfo> spotLightInfos;
		vector<SpotLight*> spotLights;

		Shader* shader;
		GLint shaderViewId;
		GLint shaderProjectionId;
		GLint shaderModelId;
		GLint shaderLightSpaceMatrixId;

		GLint materialDiffuseUniform;
		GLint materialSpecularUniform;
		GLint materialShininessUniform;
		GLint viewPosUniform;

		GLint shaderLightPosId;
		GLint shaderViewPosId;

		
		GLint shaderShadowMap;

		bool lightsDirty;
	public:
		explicit RenderPass(GameEngine *gameEngine);
		~RenderPass();

		void DirtyLights();

		void RefreshLights();

		void BeforePass() override;
		void AfterPass() override;
		void Init() override;
		void SetDrawingTransform(Transformation* transformation) const;
		GLint GetDiffuseUniform(int number) const;
		GLint GetSpecularUniform(int number) const;

		void AddSpotLight(SpotLight *spotLight);
	};
}
