#pragma once
#include "glew/glew.h"
#include "Component.h"

namespace Engine {
	class SpotLight :
		public Component
	{
		int shadowMapSize;
		Camera *camera;
		GLuint depthMapFbo;
		GLuint depthMap;
		float near;
		float far;
	public:
		explicit SpotLight(int shadowMapSize = 1024, float near = 1.0f, float far = 1.0f);
		~SpotLight();

		void Init() override;
		void Wire() override;
	};
}

