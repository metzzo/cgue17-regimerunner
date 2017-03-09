

#include "GameEngine.h"
#include "OGLGameEngine.h"
#include "Transformation.h"
#include "OGLMaterial.h"
#include "OGLMeshRenderer.h"
#include "Entity.h"
#include "OGLCamera.h"
#include "Transformation.h"
#include "glm/gtc/matrix_transform.inl"

using namespace Engine;

int main(int argc, char **argv)
{

	GameEngine *engine = new OGLGameEngine(640, 480, string("CGUE"));

	static const float bufferData[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
	};

	auto camera = new OGLCamera(45.0f, 0.1f, 100.0f);
	engine->GetRootEntity()->CreateChild()->Add(camera);
	engine->SetMainCamera(camera);
	camera->GetTransformation()->SetRelativeMatrix(lookAt(
		vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
		vec3(0, 0, 0), // and looks at the origin
		vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	));

	auto cube = engine->GetRootEntity()->CreateChild();
	cube->Add(new OGLMaterial("materials/defaultshader.vert", "materials/defaultshader.frag"));
	cube->Add(new OGLMeshRenderer(bufferData, 3));
	
	engine->Run();

	delete engine;

	return 0;
}
