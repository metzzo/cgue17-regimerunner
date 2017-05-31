#pragma once
#include "Pass.h"
#include "glew/glew.h"

namespace Engine {
	class Shader;
	class DirectionalLight;
	class SpotLight;
	class RenderPass;
	class BaseLight;

	class LightInfo
	{
		friend RenderPass;

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

	public:
		void AssignUniforms(GLuint programId, string name, int lightId, BaseLight *light);
	};

	class RenderPass : public Pass
	{
		vector<LightInfo> spotLightInfos;
		vector<SpotLight*> spotLights;

		DirectionalLight *directionalLight;
		LightInfo directionalLightInfo;

		Shader* shader;
		GLint shaderViewId;
		GLint shaderProjectionId;
		GLint shaderModelId;
		GLint shaderLightSpaceMatrixId;

		GLint materialDiffuseUniform;
		GLint materialSpecularUniform;
		GLint materialShininessUniform;
		GLint viewPosUniform;
		GLint shadowMapUniform;

		GLint shaderViewPosId;

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
		void SetDirectionalLight(DirectionalLight *directionalLight);
	};
}
