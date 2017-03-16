#pragma once
#include "Operation.h"

namespace Engine {
	class Pass
	{
	protected:
		vector<Operation*> operations;
		GameEngine *gameEngine;
	public:
		explicit Pass(GameEngine *gameEngine);
		virtual ~Pass();

		void AddOperation(Operation *operation);
		// TODO: add RemoveOperation
		vector<Operation*>& GetOperations();

		void DoPass();
		virtual void BeforePass();
		virtual void AfterPass();
		virtual void Init();
		virtual void DeInit();
	};
}
