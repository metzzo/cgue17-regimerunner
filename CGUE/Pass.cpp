#include "Pass.h"
#include <algorithm>


namespace Engine {
	bool PrioritySortFunc(Operation *a, Operation *b)
	{
		return a->GetPriority() < b->GetPriority();
	}

	Pass::Pass(GameEngine *gameEngine)
	{
		this->gameEngine = gameEngine;
		this->operations = new vector<Operation*>;
	}


	Pass::~Pass()
	{
		for (auto &operation : *operations)
		{
			delete operation;
		}
		delete operations;
	}

	void Pass::AddOperation(Operation* operation)
	{
		this->operations->push_back(operation);
		operation->SetPass(this);

		// TODO: could be optimized by inserting operation into correct position
		sort(operations->begin(), operations->end(), PrioritySortFunc);
	}

	vector<Operation*>& Pass::GetOperations() const
	{
		return *operations;
	}

	void Pass::DoPass()
	{
		BeforePass();
		for (auto &operation : *operations)
		{
			operation->Execute();
		}
		AfterPass();
	}

	void Pass::BeforePass()
	{
		// nothing!
	}

	void Pass::AfterPass()
	{
		// nothing!
	}

	void Pass::Init()
	{
		// nothing!
	}

	void Pass::DeInit()
	{
		// nothing!
	}
}

