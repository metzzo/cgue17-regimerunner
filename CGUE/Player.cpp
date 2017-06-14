#include "Player.h"
#include "Transformation.h"
#include "GameEngine.h"
#include "RenderPass.h"
#include <iostream>

namespace Game {
	const Player PlayerClass;

	void PlayerOperation::Execute()
	{
		auto component = static_cast<Player*>(this->GetComponent());

		component->blood -= component->GetEngine()->GetDeltaTime()*0.00005f;
		if (component->blood < 0)
		{
			component->blood = 0.0f;
		}

		component->GetEngine()->GetRenderPass()->SetBlood(component->blood);
	}

	Player::Player()
	{
		this->blood = 0.0f;
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

	physx::PxF32 Player::GetHeight()
	{
		return 8.0f;
	}

	physx::PxF32 Player::GetRadius()
	{
		return 1.0f;
	}

	void Player::Init()
	{

		GetEngine()->GetUpdatePass()->AddOperation(new PlayerOperation(this));
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
}
