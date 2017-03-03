#pragma once
#include "Eigen/Dense"

using namespace Eigen;
using namespace std;

namespace Engine {
	class GameState;

	class GameEngine
	{
		Vector2i screenSize;
		string programName;
		bool cancelled;
		GameState *currentGameState;

	protected:
		void RaiseEngineError(const string error);
		virtual void Init() = 0;
		virtual void DeInit() = 0;

		virtual void Render() = 0;
	public:
		GameEngine(Vector2i screenSize, string programName, GameState *gameState);
		virtual ~GameEngine();

		void Run();

		Vector2i GetScreenSize() const;
		string GetProgramName() const;
	};

}

