
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
#include "WoodInteraction.h"
#include <random>
#include "HelicopterBehaviour.h"
#include "HeightMapResource.h"
#include "DirectionalLight.h"
#include "SpriteResource.h"
#include "WaterSurface.h"
#include "Player.h"
#include "BoxGeometry.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "CollectingPlaceBehaviour.h"

using namespace Engine;
using namespace std;

HeightMapResource *PlaceMap(Entity *child)
{
	auto mapSize = vec3(256 * 8.0f, 128, 256 * 8.0f);
	auto sandTex = new TextureResource("textures/sandtext.jpg");
	auto grassTex = new TextureResource("textures/grasstext.jpg");
	auto stoneTex = new TextureResource("textures/stonetext.jpg");

	auto res = new TextureResource("textures/heightmap.png", false);
	res->Init();

	auto mapResource = new HeightMapResource(res, mapSize, 4096, 4096);
	mapResource->AddTexture(sandTex, 0, 0.5);
	mapResource->AddTexture(grassTex, 0.4, 0.75);
	mapResource->AddTexture(stoneTex, 0.6, 1.0);

	child->CreateChild()->Add(new Model(mapResource));

	auto tileWidth = res->GetWidth() / 8;
	auto tileHeight = res->GetHeight() / 8;

 	for (auto x = 0;  x < res->GetWidth(); x += tileWidth)
	{
		for (auto y = 0;  y < res->GetHeight(); y += tileHeight)
		{
			auto newRes = res->CutoutTexture(x, y, tileWidth, tileHeight);

			auto collisionMap = child->CreateChild();
			collisionMap->GetTransformation()->Translate(vec3(x / float(res->GetWidth())*mapSize.x, 0, y / float(res->GetHeight())*mapSize.z));

			auto rigidBody = new RigidBody();
			rigidBody->SetStaticness(true);
			rigidBody->SetMaterial(0.5, 0.5, 0.5);
			rigidBody->SetDensity(10);
			rigidBody->AddGeometry(new HeightFieldGeometry(newRes, vec3(mapSize.x/8.0f, mapSize.y, mapSize.z/8.0f)));
			collisionMap->Add(rigidBody);
		}
	}
	return mapResource;
}

void PlacePalm(Entity *child, vec3 pos, ModelResource *palmResource, HeightMapResource *map, Game::Player *player)
{
	auto palm = child->CreateChild();
	palm->Add(new Model(palmResource));
	palm->GetTransformation()->Translate(pos);

	auto trunk = new Engine::CapsuleGeometry(4, 30);
	auto transform = physx::PxTransform();
	transform.q = PxQuat(PxHalfPi, PxVec3(0, 0, 1));
	transform.p = PxVec3(0, 25, 0);
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

	Camera *viewCamera = nullptr;
	if (!broken) {
		viewCamera = new Camera(20.0f, 1.0f, 300.0f, 512, 512);
		viewCamera->RenderingEnabled(false);
		//viewCamera->SetCameraMode(CM_REFRACTION);
		heli->CreateChild()->Add(viewCamera);
	}

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
		heli->GetTransformation()->Translate(vec3(0, 4, 0));
		heli->GetTransformation()->Rotate(-30, vec3(0, 0, 1));

		auto rigidBody = new Engine::RigidBody();
		auto geometry = new BoxGeometry(vec3(20, 60, 60));
		auto transform = physx::PxTransform();
		transform.q = PxQuat(0, PxVec3(0, 0, 1));
		transform.p = PxVec3(0, 0, 0);
		geometry->SetLocalPose(transform);
		rigidBody->AddGeometry(geometry);
		rigidBody->SetStaticness(true);
		rigidBody->SetDensity(50);
		rigidBody->SetMaterial(0.5, 0.5, 0.5);
		heli->Add(rigidBody);
	} else
	{
		heliModel->GetTransformation()->Translate(vec3(0, 12, 0));
	}
}

void PlaceCollectingPlace(
	Entity* entity, 
	ModelResource* collectingPlaceResource, 
	vec3 pos, 
	HeightMapResource* mapResource,
	Game::Player* player)
{
	auto model = new Model(collectingPlaceResource);
	model->SetAlpha(0.25);

	auto place = entity->CreateChild();
	place->Add(model);
	place->GetTransformation()->Translate(pos + vec3(0,10, 0));
	place->GetTransformation()->Scale(vec3(10, 10, 10));
	place->Add(new Game::CollectingPlaceBehaviour(player));

}

void PlaceStone(Entity* entity, ModelResource* stone_resource, const vec3& pos)
{
	auto stone = entity->CreateChild();
	stone->GetTransformation()->Translate(pos);
	stone->GetTransformation()->Rotate(rand() % 360, vec3(0, 1, 0));
	auto stoneModel = stone->CreateChild();
	stoneModel->Add(new Model(stone_resource));
	auto s = 5 + 10.0f/(1 + rand() % 10);
	stoneModel->GetTransformation()->Scale(vec3(s, s, s));

	auto rigidBody = new Engine::RigidBody();
	auto geometry = new BoxGeometry(vec3(3, 3, 3)*s);
	rigidBody->AddGeometry(geometry);
	rigidBody->SetStaticness(true);
	rigidBody->SetDensity(50);
	rigidBody->SetMaterial(0.5, 0.5, 0.5);
	stone->Add(rigidBody);
}

void PlaceWood(Entity* entity, ModelResource* woodResource, Game::Player *player, const vec3& pos)
{
	auto wood = entity->CreateChild();
	auto woodModel = wood->CreateChild();
	auto modelComponent = new Model(woodResource);
	woodModel->Add(modelComponent);

	wood->GetTransformation()->Translate(pos);


	auto box = new Engine::BoxGeometry(vec3(90, 6, 20));
	box->SetLocalPose(physx::PxTransform(PxIdentity));

	auto rigidBody = new Engine::RigidBody();
	rigidBody->AddGeometry(box);
	rigidBody->SetStaticness(true);
	rigidBody->SetDensity(50);
	rigidBody->SetMaterial(0.5, 0.5, 0.5);
	wood->Add(rigidBody);


	wood->Add(new Game::WoodInteraction(player, modelComponent));
}

bool string2bool(std::string var)
{
	if (var == "true" || var == "TRUE")
		return true;
	else if (var == "false" || var == "FALSE")
		return false;
}


int main(int argc, char **argv)
{
	std::ifstream file("config/vars.txt");
	std::string str;
	std::getline(file, str);
	int width = std::stoi(str);
	cout << "Initializing with WIDTH:" << str << endl;
	std::getline(file, str);
	int height = std::stoi(str);
	cout << "Initializing with HEIGHT:" << str << endl;
	std::getline(file, str);
	float lightintensity = std::stoi(str)/10;
	cout << "Initializing with GAMMA:" << str << endl;
	std::getline(file, str);
	bool fullscreen = string2bool(str);
	cout << "Initializing in WINDOWMODE:" << str << endl;

	auto engine = new GameEngine(width, height, fullscreen, string("Regime Runner"));

	auto palmResource = new ModelResource("objects/palm/palmtree.obj");
	auto heliResource = new ModelResource("objects/heli2/body.obj");
	auto heliMainRotorResource = new ModelResource("objects/heli2/main_rotor.obj");
	auto heliSideRotorResource = new ModelResource("objects/heli2/side_rotor.obj");
	auto collectingPlaceResource = new ModelResource("objects/arrow/arrow.obj");
	auto stoneResource = new ModelResource("objects/rock/rock.obj");
	auto woodResource = new ModelResource("objects/wood/wood.obj");

	///*
	auto dirLight = new DirectionalLight();
	auto dirLightEntity = engine->GetRootEntity()->CreateChild();
	dirLightEntity->Add(dirLight);
	dirLightEntity->GetTransformation()->Translate(vec3(256, 256, 512));
	dirLight->SetAmbient(vec3(0 + lightintensity, 0 + lightintensity, 0 + lightintensity));
	dirLight->SetSpecular(vec3(0.2f, 0.2f, 0.2f));
	dirLight->SetDiffuse(vec3(0.2f, 0.2f, 0.2f));
	dirLight->SetLookAtVector(vec3(0, 0, 0));/**/

	auto camera = new Camera(80.0f, 0.1f, 800.0f, engine->GetScreenWidth(), engine->GetScreenHeight());
	camera->SetHudProjectionMatrix(glm::ortho(0.0f, GLfloat(engine->GetScreenWidth()), GLfloat(engine->GetScreenHeight()), 0.0f, -1.0f, 1.0f));
	auto player = engine->GetRootEntity()->CreateChild();
	auto playerComponent = new Game::Player();
	player->Add(playerComponent);
	player->Add(camera);

	auto reflectionCamera = new Camera(80.0f, 0.1f, 700.0f, engine->GetScreenWidth(), engine->GetScreenHeight());
	reflectionCamera->SetHudEnabled(false);
	reflectionCamera->SetCameraMode(CM_REFLECTION);
	reflectionCamera->SetAsReflectionCamera(true);
	reflectionCamera->SetUpVector(vec3(0.0, 1.0, 0.0));
	reflectionCamera->SetClipping(true, vec4(0, 1, 0, -20));
	player->CreateChild()->Add(reflectionCamera);


	auto refractionCamera = new Camera(80.0f, 0.1f, 700.0f, engine->GetScreenWidth(), engine->GetScreenHeight());
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

	//auto skybox = engine->GetRootEntity()->CreateChild();
	auto skyboxmodel = new ModelResource("objects/skybox/skybox.obj");
	auto skybox = player->CreateChild()->Add(new Model(skyboxmodel,true));

	auto light = player->CreateChild();
	light->GetTransformation()->Translate(vec3(0, 2.5, 0));

	auto spotLight = new SpotLight(10.0f, 35.0f);
	spotLight->SetAmbient(vec3(0, 0, 0));
	spotLight->SetSpecular(vec3(1, 1, 1));
	spotLight->SetDiffuse(vec3(0.9f, 0.9f, 0.9f));
	spotLight->SetLinear(0.007f);
	spotLight->SetQuadratic(0.002f);
	light->Add(spotLight);
	player->Add(new Game::CameraMovement(spotLight,reflectionCamera, refractionCamera, playerComponent, nullptr));

	engine->SetMainCamera(camera);
	camera->SetLookAtVector(vec3(0.0, 0.0, 0.0));

	auto mapResource = PlaceMap(engine->GetRootEntity());

	auto objectMap = new TextureResource("textures/objectmap.png", false);
	objectMap->Init();

	for (auto x = 0;  x < objectMap->GetWidth(); x++)
	{
		for (auto y = 0;  y < objectMap->GetHeight(); y++)
		{
			GLubyte r, g, b;
			objectMap->GetRgb(x, y, &r, &g, &b);

			if (r == 0 && g == 0 && b == 0)
			{
				continue;
			}

			auto pos = vec3(
				x/float(objectMap->GetWidth())*mapResource->GetSize().x, 
				mapResource->GetHeightAt(x*(mapResource->GetSize().x / objectMap->GetWidth()),y*(mapResource->GetSize().z / objectMap->GetHeight())),
				y/float(objectMap->GetHeight())*mapResource->GetSize().z
			);

			if (r == 255 && g == 0 && b == 0)
			{
				// place palm
				PlacePalm(engine->GetRootEntity(), pos, palmResource, mapResource, playerComponent);
			} else if (r == 255 && g == 255 && b == 0)
			{
				// start location
				player->GetTransformation()->Translate(pos+vec3(0,20,0));
				skybox->GetTransformation()->Translate(vec3(0, 0, -250));
				skybox->GetTransformation()->Scale(vec3(280.0, 280.0, 280.0));
			} else if (r == 0 && g == 0 && b == 255)
			{
				// broken heli
				PlaceHeli(engine->GetRootEntity(), heliResource, heliMainRotorResource, heliSideRotorResource, pos, true, mapResource, playerComponent);
			} else if (r == 0 && g == 255 && b == 0)
			{
				// normal heli
				PlaceHeli(engine->GetRootEntity(), heliResource, heliMainRotorResource, heliSideRotorResource, pos, false, mapResource, playerComponent);
			} else if (r == 0 && g ==255 && b == 255)
			{
				PlaceCollectingPlace(engine->GetRootEntity(), collectingPlaceResource, pos, mapResource, playerComponent);
			} else if (r == 255 && g == 0 && b == 255)
			{
				PlaceStone(engine->GetRootEntity(), stoneResource, pos);
			} else if (r == 255 && g == 255 && b == 255)
			{
				PlaceWood(engine->GetRootEntity(), woodResource, playerComponent, pos);
			} else
			{
				RaiseEngineError("Unknown object placed");
			}
		}
	}

	//PlaceHeli(engine->GetRootEntity(), heliResource, heliMainRotorResource, heliSideRotorResource, vec3(60, 26, 60), true, mapResource, playerComponent);

	

	
	//auto hudTest = engine->GetRootEntity()->CreateChild();
	//hudTest->GetTransformation()->Scale(vec3(0.5, 0.5, 1));
	//hudTest->GetTransformation()->Translate(vec3(500, 500, 0));
	//auto spriteResource = new SpriteResource(refractionCamera);
	//hudTest->Add(new Model(spriteResource));
	

	engine->Run();

	delete engine;

	return 0;
}
