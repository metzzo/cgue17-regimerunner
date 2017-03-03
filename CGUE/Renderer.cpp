#include "Renderer.h"


namespace Engine {
	Renderer::Renderer()
	{
		this->entity = nullptr;
	}


	Renderer::~Renderer()
	{
	}

	Entity* Renderer::getEntity() const
	{
		return this->entity;
	}

	void Renderer::Init(GameEngine* gameEngine)
	{
		// nothing \o/
	}

	void Renderer::Wire()
	{
		// nothing \o/
	}
}
