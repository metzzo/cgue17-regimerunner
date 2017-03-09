#include "OGLCamera.h"
#include "Entity.h"
#include "GameEngine.h"

namespace Engine {
	OGLCamera::~OGLCamera()
	{
	}

	void OGLCamera::RenderScreen()
	{
		// TODO: glViewport to set viewport of Camera
		this->GetEngine()->GetRootEntity()->Render();
	}
}
