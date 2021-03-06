#pragma once
#include "Operation.h"
#include <vector>


using namespace std;

namespace Engine {
	class Pass
	{
	protected:
		vector<Operation*>* operations;
		GameEngine *gameEngine;
	public:
		explicit Pass(GameEngine *gameEngine);
		virtual ~Pass();

		void AddOperation(Operation *operation);
		// TODO: add RemoveOperation
		vector<Operation*>& GetOperations() const;

		void DoPass();
		virtual void BeforePass();
		virtual void AfterPass();
		virtual void Init();
		virtual void DeInit();
	};
}
