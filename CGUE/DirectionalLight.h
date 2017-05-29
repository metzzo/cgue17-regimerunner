#pragma once
#include "BaseLight.h"
#include "Camera.h"

namespace Engine {
	class DirectionalLight :
		public BaseLight
	{
		int shadowMapSize;
		Camera *camera;
		mat4 projectionMatrix;

	public:
		explicit DirectionalLight(mat4 projectionMatrix, int shadowMapSize = 1024);
		~DirectionalLight();

		Camera *GetCamera() const;

		void Init() override;
		void AttachedToEntity() override;
	};
}