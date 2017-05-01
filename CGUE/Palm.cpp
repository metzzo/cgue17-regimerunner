#include "Entity.h"
#include "Palm.h"
#include "ModelResource.h";
#include "Model.h"
#include "GameEngine.h"
#include "Transformation.h"
#include "RigidBody.h"
#include <random>

using namespace Engine;

namespace Game {

	void Palm::PlaceRandom(int width, int length)
	{
		std::random_device rd; 
		std::mt19937 eng(rd()); 
		std::uniform_int_distribution<> distr(width, length); 

		auto x = distr(eng)*1.0f;
		auto z = distr(eng)*1.0f;
		this->GetTransformation()->Translate(vec3(x,-3.0f,z));	
		this->GetTransformation()->Scale(vec3(10.0f, 10.0f, 10.0f));
	}
}