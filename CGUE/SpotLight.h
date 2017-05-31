#pragma once
#include "BaseLight.h"

namespace Engine {
	class SpotLight :
		public BaseLight
	{
		float cutOff;
		float outerCutOff;
		vec3 lookAtVector;
	public:
		SpotLight(); // do not use this constructor

		explicit SpotLight(float cutOff, float outerCutoff);
		~SpotLight();

		void SetLookAtVector(vec3 lookAt);

		float GetCutOff() const;
		float GetOuterCutOff() const;

		void Init() override;
		vec3 GetLookAtVector() const;
	};

	extern const SpotLight SpotLightClass;
}

