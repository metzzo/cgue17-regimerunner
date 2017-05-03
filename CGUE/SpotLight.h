#pragma once
#include "Camera.h"

namespace Engine {
	class SpotLight :
		public Component
	{
		int shadowMapSize;
		Camera *camera;
		mat4 projectionMatrix;
	public:
		explicit SpotLight(mat4 projectionMatrix, int shadowMapSize = 1024);
		~SpotLight();

		Camera *GetCamera() const;

		void Init() override;
		void Wire() override;
		void AttachedToEntity() override;
	};
}

