

#include "GameEngine.h"
#include "OGLGameEngine.h"
#include "Transformation.h"
#include "OGLMaterial.h"
#include "OGLMeshRenderer.h"
#include "Entity.h"

using namespace Engine;

int main(int argc, char **argv)
{

	GameEngine *engine = new OGLGameEngine(Vector2i(640, 480), string("CGUE"));

	static const float bufferData[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
	};

	auto cube = engine->CreateEntity();
	cube->Add(new Transformation());
	cube->Add(new OGLMaterial("materials/defaultshader.vert", "materials/defaultshader.frag"));
	cube->Add(new OGLMeshRenderer(bufferData, 3));
	
	engine->Run();

	delete engine;

	return 0;
}
