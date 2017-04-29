#pragma once
#include "Component.h"
#include "RigidBody.h"

namespace Engine {
	class BaseShape : public Component
	{
	protected:
		RigidBody *rigidBody;
	public:
		BaseShape();
		~BaseShape();

		void Wire() override;
	};

}
