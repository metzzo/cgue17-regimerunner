#pragma once
#include "Eigen/Dense"
#include <vector>

using namespace Eigen;
using namespace std;

namespace Engine {
	class Entity;
	class GameState;

	class GameEngine
	{
		Vector2i screenSize;
		string programName;
		bool cancelled;

	protected:
		vector<Entity*> entities;

		void RaiseEngineError(const string error);

		virtual void Init() = 0;
		virtual void DeInit() = 0;

		virtual void Render() = 0;
	public:
		GameEngine(Vector2i screenSize, string programName);
		virtual ~GameEngine();

		void Run();

		Vector2i GetScreenSize() const;
		string GetProgramName() const;

		Entity *CreateEntity();
	};

}

