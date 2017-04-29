#pragma once
#include "BaseShape.h"
namespace Engine {
	class CapsuleShape :
		public BaseShape
	{
		float radius;
		float halfHeight;
	public:
		CapsuleShape(float radius, float halfHeight);
		~CapsuleShape();

		void Init() override;
	};
}