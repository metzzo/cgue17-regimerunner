#include "Pass.h"


namespace Engine {
	Pass::Pass(GameEngine *gameEngine)
	{
		this->gameEngine = gameEngine;
	}


	Pass::~Pass()
	{
	}

	void Pass::AddOperation(Operation* operation)
	{
		this->operations.push_back(operation);
	}

	vector<Operation*>& Pass::GetOperations()
	{
		return operations;
	}

	void Pass::DoPass()
	{
		for (auto &operation : operations)
		{
			operation->Execute();
		}
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

