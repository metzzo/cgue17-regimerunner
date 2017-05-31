#include "WaterPass.h"
#include "Shader.h"
#include "GameEngine.h"
#include "Camera.h"
#include <string>

namespace Engine {

	WaterPass::WaterPass(GameEngine * gameEngine) : Pass(gameEngine)
	{
	}

	Engine::WaterPass::~WaterPass()
	{
	}

	void Engine::WaterPass::BeforePass()
	{
	}

	void Engine::WaterPass::AfterPass()
	{
	}

	void Engine::WaterPass::Init()
	{
	}

	void Engine::WaterPass::SetDrawingTransform(Transformation * transformation)
	{
	}
}
