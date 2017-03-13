#pragma once
#include "Transformation.h"

namespace Engine {
	class Camera :
		public Component
	{
	protected:
		float fov;
		float far;
		float near;

		int width;
		int height;

		mat4x4 projectionMatrix;
		bool ortho;
	public:
		Camera();
		Camera(float fov, float near, float far, int width, int height, bool ortho = false);
		~Camera();

		float GetFov() const;
		float GetFar() const;
		float GetNear() const;

		virtual void RenderScreen();

		mat4x4 GetProjectionViewMatrix() const;

		void Wire() override;
		void Init() override;
	};

	extern const Camera CameraClass;
}
