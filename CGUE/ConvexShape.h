#pragma once
#include "BaseShape.h"
#include "RigidBody.h"

namespace Engine {
	class Model;
	class ConvexShape :
		public BaseShape
	{
		Model *model;
		RigidBody *body;
	public:
		ConvexShape();
		~ConvexShape();
		
		void Wire() override;
		void Init() override;
	};
}
