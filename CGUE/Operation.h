#pragma once
#include "Component.h"

namespace Engine {

	class Operation
	{
		Component *component;
		Pass* pass;
	public:
		explicit Operation(Pass *pass, Component *component);
		virtual ~Operation();

		virtual void Execute() = 0;
		virtual int GetPriority();

		Component *GetComponent() const;
		Pass *GetPass() const;
	};
}
