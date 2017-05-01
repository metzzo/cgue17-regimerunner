#pragma once
#include "Camera.h"

namespace Engine {
	class SpotLight :
		public Component
	{
		int shadowMapSize;
		Camera *camera;
		float near;
		float far;
	public:
		explicit SpotLight(int shadowMapSize = 1024, float near = 1.0f, float far = 10.0f);
		~SpotLight();

		Camera *GetCamera() const;

		void Init() override;
		void Wire() override;
		void AttachedToEntity() override;
	};
}

