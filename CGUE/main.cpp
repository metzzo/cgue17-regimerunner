
#include "GameEngine.h"
#include "GameEngine.h"
#include "Transformation.h"
#include "Entity.h"
#include "Camera.h"
#include "glm/gtc/matrix_transform.inl"
#include "CameraMovement.h"
#include "SpotLight.h"
#include "Model.h"
#include "ModelResource.h"
#include "CapsuleGeometry.h"
#include "HeightFieldGeometry.h"
#include "PalmInteraction.h"
#include <random>
#include "HelicopterBehaviour.h"
#include "HeightMapResource.h"
#include "DirectionalLight.h"
#include "SpriteResource.h"
#include "WaterSurface.h"
#include "Player.h"
#include "BoxGeometry.h"
#include <iostream>

using namespace Engine;

void PlaceMap(Entity *child)
{

	auto chunkSize = vec3(256, 128, 256);
	auto sandTex = new TextureResource("textures/sandtext.jpg");
	auto grassTex = new TextureResource("textures/grasstext.jpg");
	auto stoneTex = new TextureResource("textures/stonetext.jpg");

	auto res = new TextureResource("textures/heightmap.png", false);
	res->Load();
	cout << "Heightmap Loaded" << endl;

	auto tileWidth = res->GetWidth() / 8;
	auto tileHeight = res->GetHeight() / 8;

 	for (auto x = 0;  x < res->GetWidth(); x += tileWidth)
	{
		for (auto y = 0;  y < res->GetHeight(); y += tileHeight)
		{
			auto tmpX = x - 2;
			if (tmpX < 0)
			{
				tmpX = 0;
			}
			auto tmpY = y - 2;
			if (tmpY < 0)
			{
				tmpY = 0;
			}
			auto tmpWidth = tileWidth + 2;
			if (tmpWidth >= res->GetWidth())
			{
				tmpWidth = res->GetWidth() - 2;
			}
			auto tmpHeight = tileHeight + 2;
			if (tmpHeight >= res->GetHeight())
			{
				tmpHeight = res->GetHeight() - 2;
			}


			auto newRes = res->CutoutTexture(tmpX, tmpY, tmpWidth, tmpHeight);
			newRes->SetFilename("textures/tiles/tile_" + to_string(x) + "_" + to_string(y));

			auto mapResource = new HeightMapResource(newRes, chunkSize, 64, 64);
			mapResource->AddTexture(sandTex, 0, 0.3);
			mapResource->AddTexture(grassTex, 0.15, 0.5);
			mapResource->AddTexture(stoneTex, 0.3, 1.0);

			auto map = child->CreateChild();
			map->GetTransformation()->Translate(vec3(x / float(tileWidth)*chunkSize.x, 0, y / float(tileHeight)*chunkSize.z));
			map->Add(new Model(mapResource));

			auto rigidBody = new RigidBody();
			rigidBody->SetStaticness(true);
			rigidBody->SetMaterial(0.5, 0.5, 0.5);
			rigidBody->SetDensity(10);
			rigidBody->AddGeometry(new HeightFieldGeometry(mapResource->GetHeightMap(), chunkSize));
			map->Add(rigidBody);

		}
	}
}
/*
void PlacePalms(Entity *child, ModelResource *palmResource, HeightMapResource *map)
{
	auto mapSize = map->GetSize();
	for (auto x = 32; x < mapSize.x; x += 96)
	{
		for (auto z = 32; z < mapSize.z; z += 96)
		{
			auto palm = child->CreateChild();
			palm->Add(new Model(palmResource));
			palm->Add(new Game::PalmInteraction(x + (rand() % 40 - 20), z + (rand() % 40 - 20), map));

			auto trunk = new Engine::CapsuleGeometry(4, 30);
			auto transform = physx::PxTransform();
			transform.q = PxQuat(PxHalfPi, PxVec3(0, 0, 1));
			transform.p = PxVec3(0, 30, 0);
			trunk->SetLocalPose(transform);

			auto leaves = new Engine::BoxGeometry(vec3(50, 4, 50));
			transform = physx::PxTransform();
			transform.q = PxQuat(0, PxVec3(0, 0, 1));
			transform.p = PxVec3(0, 60, 0);
			leaves->SetLocalPose(transform);

			auto rigidBody = new Engine::RigidBody();
			rigidBody->AddGeometry(trunk);
			rigidBody->AddGeometry(leaves);
			rigidBody->SetStaticness(true);
			rigidBody->SetDensity(50);
			rigidBody->SetMaterial(0.5, 0.5, 0.5);
			palm->Add(rigidBody);
		}
	}
}

void PlaceHeli(
	Entity *child, 
	ModelResource *heliResource, 
	ModelResource *heliMainRotorResource, 
	ModelResource *heliSideRotorResource, 
	vec3 pos, 
	bool broken, 
	HeightMapResource *heightMap,
	Game::Player *player)
{
	auto engine = child->GetEngine();

	std::random_device rd;
	std::mt19937 eng(rd());
	uniform_int_distribution<> distr(1, 600);

	auto x = distr(eng)*1.0f;
	auto z = distr(eng)*1.0f;

	auto heli = child->CreateChild();
	auto heliModel = heli->CreateChild();

	heliModel->Add(new Model(heliResource));


	auto heliMainRotor = heliModel->CreateChild();
	heliMainRotor->GetTransformation()->Translate(vec3(-2.5, 9.67598, 9.34674));
	heliMainRotor = heliMainRotor->CreateChild();
	heliMainRotor->Add(new Model(heliMainRotorResource));

	auto heliSideRotor = heliModel->CreateChild();
	heliSideRotor->GetTransformation()->Translate(vec3(-0.1404, 5.2074, -34.8371));
	heliSideRotor = heliSideRotor->CreateChild();
	heliSideRotor->Add(new Model(heliSideRotorResource));

	heli->GetTransformation()->Translate(pos);

	auto viewCamera = new Camera(20.0f, 1.0f, 300.0f, 512, 512);
	//viewCamera->RenderingEnabled(false);
	viewCamera->SetCameraMode(CM_REFRACTION);
	heli->CreateChild()->Add(viewCamera);

	heli->Add(new Game::HelicopterBehaviour(
		heliMainRotor->GetTransformation(), 
		heliSideRotor->GetTransformation(),
		broken, 
		heightMap, 
		player,
		viewCamera));
	
	//auto hudTest = engine->GetRootEntity()->CreateChild();
	//hudTest->GetTransformation()->Scale(vec3(1, 1, 1));
	//hudTest->GetTransformation()->Translate(vec3(500, 500, 0));
	//auto spriteResource = new SpriteResource(viewCamera);
	//hudTest->Add(new Model(spriteResource));

	auto spotLight = broken ? new SpotLight(15.0f, 20.0f) : new SpotLight(80.0f, 1.0f, 1000.0f, 512, 19.0f, 20.0f);
	spotLight->SetAmbient(vec3(0, 0, 0));
	spotLight->SetSpecular(vec3(1, 1, 1));
	spotLight->SetDiffuse(vec3(0.9f, 0.9f, 0.9f));
	spotLight->SetLinear(0.0014f);
	spotLight->SetQuadratic(0.00007f);
	heli->Add(spotLight);

	if (broken)
	{
		spotLight->GetCamera()->SetLookAtVector(heli->GetTransformation()->GetAbsolutePosition() + vec3(0, -1, 2.5));
		heli->GetTransformation()->Rotate(30, vec3(0, 0, 1));
	} else
	{
		heliModel->GetTransformation()->Translate(vec3(0, 12, 0));
	}
}*/

int main(int argc, char **argv)
{
	auto engine = new GameEngine(1440, 800, string("CGUE"));

	auto palmResource = new ModelResource("objects/palm/palmtree.obj");
	auto heliResource = new ModelResource("objects/heli2/body.obj");
	auto heliMainRotorResource = new ModelResource("objects/heli2/main_rotor.obj");
	auto heliSideRotorResource = new ModelResource("objects/heli2/side_rotor.obj");

	///*
	auto dirLight = new DirectionalLight();
	auto dirLightEntity = engine->GetRootEntity()->CreateChild();
	dirLightEntity->Add(dirLight);
	dirLightEntity->GetTransformation()->Translate(vec3(256, 256, 512));
	dirLight->SetAmbient(vec3(0, 0, 0));
	dirLight->SetSpecular(vec3(0.2f, 0.2f, 0.2f));
	dirLight->SetDiffuse(vec3(0.2f, 0.2f, 0.2f));
	dirLight->SetLookAtVector(vec3(0, 0, 0));/**/

	auto camera = new Camera(80.0f, 0.1f, 500.0f, engine->GetScreenWidth(), engine->GetScreenHeight());
	camera->SetHudProjectionMatrix(glm::ortho(0.0f, GLfloat(engine->GetScreenWidth()), GLfloat(engine->GetScreenHeight()), 0.0f, -1.0f, 1.0f));
	auto player = engine->GetRootEntity()->CreateChild();
	player->GetTransformation()->Translate(vec3(100, 200, 100));
	auto playerComponent = new Game::Player();
	player->Add(playerComponent);
	player->Add(camera);

	auto reflectionCamera = new Camera(80.0f, 0.1f, 500.0f, engine->GetScreenWidth(), engine->GetScreenHeight());
	reflectionCamera->SetHudEnabled(false);
	reflectionCamera->SetCameraMode(CM_REFLECTION);
	reflectionCamera->SetAsReflectionCamera(true);
	reflectionCamera->SetUpVector(vec3(0.0, 1.0, 0.0));
	reflectionCamera->SetClipping(true, vec4(0, 1, 0, -20));
	player->CreateChild()->Add(reflectionCamera);


	auto refractionCamera = new Camera(80.0f, 0.1f, 500.0f, engine->GetScreenWidth(), engine->GetScreenHeight());
	refractionCamera->SetHudEnabled(false);
	refractionCamera->SetCameraMode(CM_REFRACTION);
	refractionCamera->SetAsRefractionCamera(true);
	refractionCamera->SetUpVector(vec3(0.0, 1.0, 0.0));
	refractionCamera->SetClipping(true, vec4(0, -1, 0, 20));
	player->CreateChild()->Add(refractionCamera);

	auto watersurface = engine->GetRootEntity()->CreateChild();
	watersurface->GetTransformation()->Translate(vec3(0, 20, 0));
	auto water = new WaterSurface();
	watersurface->Add(water);

	auto light = player->CreateChild();
	light->GetTransformation()->Translate(vec3(0, 2.5, 0));

	auto spotLight = new SpotLight(10.0f, 35.0f);
	spotLight->SetAmbient(vec3(0, 0, 0));
	spotLight->SetSpecular(vec3(1, 1, 1));
	spotLight->SetDiffuse(vec3(0.9f, 0.9f, 0.9f));
	spotLight->SetLinear(0.007f);
	spotLight->SetQuadratic(0.002f);
	light->Add(spotLight);
	player->Add(new Game::CameraMovement(spotLight,reflectionCamera, refractionCamera, playerComponent));

	engine->SetMainCamera(camera);
	camera->SetLookAtVector(vec3(0.0, 0.0, 0.0));


	//PlaceHeli(engine->GetRootEntity(), heliResource, heliMainRotorResource, heliSideRotorResource, vec3(256, 185, 256), false, mapResource, playerComponent);
	//PlaceHeli(engine->GetRootEntity(), heliResource, heliMainRotorResource, heliSideRotorResource, vec3(32, 185, 32), false, mapResource, playerComponent);
	//PlaceHeli(engine->GetRootEntity(), heliResource, heliMainRotorResource, heliSideRotorResource, vec3(60, 26, 60), true, mapResource, playerComponent);

	//PlacePalms(engine->GetRootEntity(), palmResource, mapResource);
	
	PlaceMap(engine->GetRootEntity());

	
	//auto hudTest = engine->GetRootEntity()->CreateChild();
	//hudTest->GetTransformation()->Scale(vec3(0.5, 0.5, 1));
	//hudTest->GetTransformation()->Translate(vec3(500, 500, 0));
	//auto spriteResource = new SpriteResource(refractionCamera);
	//hudTest->Add(new Model(spriteResource));
	

	engine->Run();

	delete engine;

	return 0;
}
