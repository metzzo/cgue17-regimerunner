#pragma once
#include "glew/glew.h"
#include "Component.h"
#include "Operation.h"

namespace Engine {
	class SpotLightRenderOperation : public Operation
	{
	public:
		explicit SpotLightRenderOperation(Component* component)
			: Operation(component)
		{
		}

		void Execute() override;
		QUEUE_TYPE GetQueueType() override;
	};

	class SpotLight :
		public Component
	{
		friend SpotLightRenderOperation;

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

