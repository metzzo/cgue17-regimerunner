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

		if (component->inIntro == true) {
			if (component->introDisplayed == false) {
				component->introDisplayed = true;
				auto hudTest = component->GetEngine()->GetRootEntity()->CreateChild();
				hudTest->GetTransformation()->Translate(vec3(component->GetEngine()->GetMainCamera()->GetWidth() / 2, component->GetEngine()->GetMainCamera()->GetHeight() - 200, 0));
				auto spriteResource = new SpriteResource(new TextureResource("textures/intro1.png"));
				hudTest->Add(new Model(spriteResource));
				component->intro = hudTest;
			}
		
		}

		if (component->hasEnoughWoodGathered() == true) {
			if (component->displayedScreen() == false) {
				component->win();
				auto hudTest = component->GetEngine()->GetRootEntity()->CreateChild();
				hudTest->GetTransformation()->Translate(vec3(component->GetEngine()->GetMainCamera()->GetWidth() / 2, component->GetEngine()->GetMainCamera()->GetHeight() / 2, 0));
				auto spriteResource = new SpriteResource(new TextureResource("textures/win.png"));
				hudTest->Add(new Model(spriteResource));
			}
		}

		if (component->blood > 0.2) {
			if (component->displayedScreen() == false) {
				component->lose();
				auto hudTest = component->GetEngine()->GetRootEntity()->CreateChild();
				hudTest->GetTransformation()->Translate(vec3(component->GetEngine()->GetMainCamera()->GetWidth()/2, component->GetEngine()->GetMainCamera()->GetHeight()/2, 0));
				auto spriteResource = new SpriteResource(new TextureResource("textures/lost.png"));
				auto screen = new Model(spriteResource);
				screen->SetAlpha(0.1);
				hudTest->Add(screen);
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
		this->inIntro = true;
		this->introDisplayed = false;
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
		return this->displayedLostScreen || this->displayedWinScreen;
	}

	void Player::increaseGatheredWood() {
		this->gatheredWood++;
	}

	bool Player::hasEnoughWoodGathered() {
		return this->gatheredWood > 5;
	}

	bool Player::isInIntro() {
		return this->inIntro;
	}

	void Player::leaveIntro() {
		this->inIntro = false;
		this->intro->GetTransformation()->Translate(vec3(10000,0,0));
	}
}
