#pragma once
#include "Renderer.h"
#include "Transformation.h"

namespace Engine {
	class Camera :
		public Component
	{
	protected:
		float fov;
		float far;
		float near;

		mat4x4 projectionMatrix;
	public:
		Camera();
		Camera(float fov, float near, float far);
		~Camera();

		virtual void RenderScreen() = 0;

		mat4x4 GetProjectionViewMatrix() const;
		void UpdateProjectionView() const;

		void Wire() override;
		void Init() override;
		
	};
}
