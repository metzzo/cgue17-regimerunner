#include "TestGameState.h"
#include "Entity.h"
#include "GameEngine.h"
#include "OGLMeshRenderer.h"
#include "Transformation.h"

namespace Game {
	void TestGameState::Init(GameEngine* engine)
	{
		static const float bufferData[] = {
			-1.0f, -1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			0.0f,  1.0f, 0.0f,
		};

		auto cube = engine->CreateEntity();
		cube->AddComponent(new Transformation());

		cube->AddComponent(new OGLMaterial("materials/defaultshader.vert", "materials/defaultshader.frag"));

		auto renderer = new OGLMeshRenderer(bufferData, 3);
		cube->AddRenderer(renderer);
	}
}
