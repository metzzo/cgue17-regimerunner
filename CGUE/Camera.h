#pragma once
#include "Renderer.h"
#include "Transformation.h"

namespace Engine {
	class Camera :
		public Renderer
	{
	protected:
		Transformation transform;

	public:
		Camera();
		~Camera();

		void Wire() override;
	};
}
