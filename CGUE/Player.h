#pragma once
#include "Component.h"
#include "Frustum.h"
#include <foundation/Px.h>
#include "Operation.h"
#include <PxActor.h>
#include "CameraMovement.h"
#include "SpriteResource.h"
#include "Model.h"
#include "TextureResource.h"
#include "ModelResource.h"
#include "Entity.h"

namespace Game {
	class WoodInteraction;

	class PlayerOperation : public Engine::Operation
	{

	public:
		explicit PlayerOperation(Engine::Component* component)
			: Operation(component)
		{
		}

		void Execute() override;
	};

	class Player : public Engine::Component
	{
		friend PlayerOperation;

		double blood;
		bool lost;
		bool displayedLostScreen;
		bool won;
		bool displayedWinScreen;
		bool introDisplayed;
		bool inIntro;
		int gatheredWood;
		CameraMovement *cameraMovement;
		WoodInteraction *wood;
		Engine::Entity *intro;
	public:
		Player();
		~Player();
		Engine::AABox GetAABB() const;
		physx::PxActor *GetActor() const;
		WoodInteraction *GetWood() const;
		static physx::PxF32 GetHeight();
		static physx::PxF32 GetRadius();

		void SetWood(WoodInteraction *wood);

		void Init() override;
		void Wire() override;
		void HelicopterHit();


		void lose();
		void win();
		bool hasLost();
		bool hasWon();
		bool displayedScreen();
		bool hasEnoughWoodGathered();
		void increaseGatheredWood();

		bool isInIntro();
		void leaveIntro();

	};
	extern const Player PlayerClass;
}

