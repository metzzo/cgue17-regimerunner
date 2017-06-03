#pragma once
#include "BaseLight.h"
#include "Camera.h"

namespace Engine {
	class SpotLight :
		public BaseLight
	{
		float cutOff;
		float outerCutOff;
		vec3 lookAtVector;
		bool shadowCasting;


		int shadowMapSize;
		Camera *camera;
		mat4 projectionMatrix;
	public:
		SpotLight(); // do not use this constructor

		explicit SpotLight(float cutOff, float outerCutoff);
		explicit SpotLight(mat4 projectionMatrix, int shadowMapSize, float cutOff, float outerCutoff);
		~SpotLight();

		float GetCutOff() const;
		float GetOuterCutOff() const;

		void Init() override;
		void AttachedToEntity() override;
		
		bool IsShadowCasting() const;
		Camera* GetCamera() const;
	};

	extern const SpotLight SpotLightClass;
}

