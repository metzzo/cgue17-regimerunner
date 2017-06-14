#pragma once
#include "Pass.h"
#include "glew/glew.h"

#define MAX_NUM_SHADOW_MAPS 10

namespace Engine {
	class Shader;
	class DirectionalLight;
	class SpotLight;
	class RenderPass;
	class BaseLight;
	class Timer;

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

		GLint shadowCastingUniform;
		GLint shadowMapIndexUniform;
		GLint spaceMatrixUniform;
	public:
		void AssignUniforms(GLuint programId, string name, int lightId, BaseLight *light);
	};

#define NUM_SHADOW_MAPS 16

	class RenderPass : public Pass
	{
		vector<LightInfo> spotLightInfos;
		vector<SpotLight*> spotLights;

		DirectionalLight *directionalLight;
		LightInfo directionalLightInfo;

		Shader* shader;
		GLint viewUniform;
		GLint projectionUniform;
		GLint hudProjectionUniform;
		GLint modelUniform;
		GLint renderTypeUniform;

		GLint WaterNormalMapUniform;
		GLint WaterUVDVMapUniform;
		GLint WaterReflectionUniform;
		GLint WaterRefractionUniform;
		GLint EyeTanSpaceUniform;
		GLint LightTanSpaceUniform;
		GLint waveOffsetUniform;
		GLint texOffsetUniform;

		GLint materialDiffuseUniform;
		GLint materialSpecularUniform;
		GLint materialShininessUniform;
		GLint viewPosUniform;
		GLint shadowMapUniform[NUM_SHADOW_MAPS];
		GLint clippingPlaneUniform;
		GLint enableClippingUniform;
		GLint bloodUniform;

		bool lightsDirty;
		int numShadowMaps;

		GLint reflectionTexture;
		GLint refractionTexture;
		float blood;
	public:
		explicit RenderPass(GameEngine *gameEngine);
		~RenderPass();

		void DirtyLights();

		void RefreshLights();

		void BeforePass() override;
		void AfterPass() override;
		void Init() override;
		void SetDrawingTransform(Transformation* transformation) const;
		void SetBlood(float blood);
		GLint GetDiffuseUniform(int number) const;
		GLint GetSpecularUniform(int number) const;
		int GetNumShadowMaps() const;

		GLint GetWaterNormalMapUniform() const;
		GLint GetWaterUVDVMapUniform() const;
		GLint GetWaterReflectionUniform() const;
		GLint GetWaterRefractionUniform() const;

		GLint GetWaveOffsetUniform() const;
		GLint GetTexOffsetUniform() const;

		void AddSpotLight(SpotLight *spotLight);
		void SetDirectionalLight(DirectionalLight *directionalLight);

		GLint GetRenderTypeUniform() const;

		void SetReflectionTexture(GLint reflectionTexture);
		void SetRefractionTexture(GLint refractionTexture);
	};
}
