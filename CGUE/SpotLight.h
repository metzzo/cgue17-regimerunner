#pragma once
#include "glew/glew.h"
#include "Component.h"
#include "Operation.h"

namespace Engine {
	class LightRenderOperation : public Operation
	{
	public:
		explicit LightRenderOperation(Component* component)
			: Operation(component)
		{
		}

		void Execute() override;
		OPERATION_TYPE GetOperationType() override;
	};

	class SpotLight :
		public Component
	{
		friend LightRenderOperation;

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
	extern SpotLight *spotlight;
}

