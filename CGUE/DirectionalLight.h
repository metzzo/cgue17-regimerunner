#pragma once
#include "BaseLight.h"
#include "Camera.h"

namespace Engine {
	class DirectionalLight :
		public BaseLight
	{
		vec3 lookAtVector;
	public:
		explicit DirectionalLight();
		~DirectionalLight();

		void Init() override;

		void SetLookAtVector(vec3 lookAt);
		vec3 GetLookAtVector() const;
	};
}