#pragma once
#include "Camera.h"

namespace Engine {
	class OGLCamera :
		public Camera
	{

	public:
		using Camera::Camera;
		~OGLCamera();

		void RenderScreen() override;
	};
}
