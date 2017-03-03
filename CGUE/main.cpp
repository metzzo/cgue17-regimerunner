

#include "GameEngine.h"
#include "OGLGameEngine.h"
#include "TestGameState.h"

using namespace Engine;
using namespace Game;

int main(int argc, char **argv)
{
	TestGameState testGameState;

	GameEngine *engine = new OGLGameEngine(Vector2i(640, 480), string("CGUE"), &testGameState);
	
	engine->Run();

	delete engine;

	return 0;
}
