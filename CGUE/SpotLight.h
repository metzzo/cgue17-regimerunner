#pragma once
#include "Camera.h"
#include "BaseLight.h"

namespace Engine {
	class SpotLight :
		public BaseLight
	{
		int shadowMapSize;
		Camera *camera;
		mat4 projectionMatrix;
		float cutOff;
		float outerCutOff;
		bool shadowCasting;
	public:
		explicit SpotLight(mat4 projectionMatrix, int shadowMapSize = 1024, float cutOff = 0.0f, float outerCutoff = 0.05f);
		~SpotLight();

		Camera *GetCamera() const;
		float GetCutOff() const;
		float GetOuterCutOff() const;
		bool IsShadowCasting() const;

		void Init() override;
		void Wire() override;
		void AttachedToEntity() override;
	};
}

