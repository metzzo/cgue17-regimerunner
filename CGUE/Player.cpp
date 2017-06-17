#include "Player.h"
#include "Transformation.h"
#include "GameEngine.h"
#include "RenderPass.h"
#include <iostream>
#include "Entity.h"
#include "RigidBody.h"
#include "WoodInteraction.h"
#include "SpriteResource.h"
#include "Model.h"
#include "TextureResource.h"
#include "ModelResource.h"

using namespace Engine;

namespace Game {
	const Player PlayerClass;

	void PlayerOperation::Execute()
	{
		auto component = static_cast<Player*>(this->GetComponent());

		component->blood -= component->GetEngine()->GetDeltaTime()*0.00005f;

		if (component->hasEnoughWoodGathered() == true) {
			if (component->displayedScreen() == false) {
				component->win();
				auto hudTest = component->GetEngine()->GetRootEntity()->CreateChild();
				hudTest->GetTransformation()->Translate(vec3(component->GetEngine()->GetMainCamera()->GetWidth() / 2, component->GetEngine()->GetMainCamera()->GetHeight() / 2, 0));
				auto spriteResource = new SpriteResource(new TextureResource("textures/win.jpg"));
				hudTest->Add(new Model(spriteResource));
			}
		}

		if (component->blood > 0.3) {
			if (component->displayedScreen() == false) {
				component->lose();
				auto hudTest = component->GetEngine()->GetRootEntity()->CreateChild();
				hudTest->GetTransformation()->Translate(vec3(component->GetEngine()->GetMainCamera()->GetWidth()/2, component->GetEngine()->GetMainCamera()->GetHeight()/2, 0));
				auto spriteResource = new SpriteResource(new TextureResource("textures/lost.jpg"));
				hudTest->Add(new Model(spriteResource));
			}
		}

		/*
		if(player -> enough wood) {
		
		}		
		*/

		if (component->blood < 0)
		{
			component->blood = 0.0f;
		}

		component->GetEngine()->GetRenderPass()->SetBlood(component->blood);
	}

	Player::Player()
	{
		this->blood = 0.0f;
		this->wood = nullptr;
		this->won = false;
		this->lost = false;
		this->gatheredWood = 0;
		// are set to true to prevent adding numerous winning and lost screens
		this->displayedLostScreen = false;
		this->displayedWinScreen = false;
	}


	Player::~Player()
	{
	}

	Engine::AABox Player::GetAABB() const
	{
		auto pos = GetTransformation()->GetAbsolutePosition();
		auto half = vec3(GetRadius(), GetHeight() / 2, GetRadius())/10.0f;
		return Engine::AABox(pos - half, pos.x + half.x, pos.y + half.y, pos.z + half.z);
	}

	physx::PxActor* Player::GetActor() const
	{
		return cameraMovement->GetController()->getActor();
	}

	WoodInteraction* Player::GetWood() const
	{
		return this->wood;
	}

	physx::PxF32 Player::GetHeight()
	{
		return 8.0f;
	}

	physx::PxF32 Player::GetRadius()
	{
		return 1.0f;
	}

	void Player::SetWood(WoodInteraction* wood)
	{
		this->wood = wood;
	}

	void Player::Init()
	{

		GetEngine()->GetUpdatePass()->AddOperation(new PlayerOperation(this));
	}

	void Player::Wire()
	{
		WIRE_COMPONENT(this->cameraMovement, CameraMovementClass);
	}

	void Player::HelicopterHit()
	{
		this->blood += GetEngine()->GetDeltaTime()*0.0001f;

		if (this->blood > 0.75f)
		{
			this->blood = 0.75f;
		}

		GetEngine()->GetRenderPass()->SetBlood(blood);
	}

	void Player::lose() {
		this->lost = true;
		this->displayedLostScreen = true;
	}

	void Player::win() {
		this->won = true;
		this->displayedWinScreen = true;
	}

	bool Player::hasLost() {
		return this->lost;
	}

	bool Player::hasWon() {
		return this->won;
	}

	bool Player::displayedScreen() {
		if (this->displayedLostScreen || this->displayedWinScreen) return true;
		return false;
	}

	void Player::increaseGatheredWood() {
		this->gatheredWood++;
	}

	bool Player::hasEnoughWoodGathered() {
		if (this->gatheredWood > 7) return true;
		return false;
	}
}
