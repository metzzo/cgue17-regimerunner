
#include "GameEngine.h"
#include "GameEngine.h"
#include "Transformation.h"
#include "Entity.h"
#include "Camera.h"
#include "Transformation.h"
#include "glm/gtc/matrix_transform.inl"
#include "CameraMovement.h"
#include "SpotLight.h"
#include "Model.h"
#include "ModelResource.h"
#include "ConvexShape.h"
#include "CapsuleShape.h"
#include "HeightFieldShape.h"
#include "PalmInteraction.h"
#include <random>
#include "HelicopterBehaviour.h"
#include "HeightMapResource.h"
#include "DirectionalLight.h"
#include "SpriteResource.h"
#include "WaterSurface.h"
#include "Player.h"

using namespace Engine;

void PlacePalms(Entity *child, ModelResource *palmResource, HeightMapResource *map)
{
	auto mapSize = map->GetSize();
	for (auto x = 32; x < mapSize.x; x += 64)
	{
		for (auto z = 32; z < mapSize.z; z += 64)
		{
			auto palm = child->CreateChild();
			palm->Add(new Model(palmResource));
			palm->Add(new Game::PalmInteraction(x + (rand() % 40 - 20), z + (rand() % 40 - 20), map));

			auto shape = new Engine::CapsuleShape(4, 30);
			palm->Add(shape);

			auto rigidBody = new Engine::RigidBody();
			auto transform = PxTransform();
			transform.q = PxQuat(PxHalfPi, PxVec3(0, 0, 1));
			transform.p = PxVec3(0, 30, 0);
			rigidBody->SetLocalPose(transform);
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
	heli->Add(new Game::HelicopterBehaviour(heliMainRotor->GetTransformation(), heliSideRotor->GetTransformation(), broken, heightMap, player));

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
}

int main(int argc, char **argv)
{
	auto engine = new GameEngine(1440, 800, string("CGUE"));
	auto mapSize = vec3(512, 128, 512);
	auto mapResource = new HeightMapResource("textures/heightmap.png", mapSize, 2048, 2048);
	mapResource->AddTexture(new TextureResource("textures/sandtext.jpg"), 0, 0.3);
	mapResource->AddTexture(new TextureResource("textures/grasstext.jpg"), 0.15, 0.5);
	mapResource->AddTexture(new TextureResource("textures/stonetext.jpg"), 0.3, 1.0);

	auto palmResource = new ModelResource("objects/palm/palmtree.obj");
	auto heliResource = new ModelResource("objects/heli2/body.obj");
	auto heliMainRotorResource = new ModelResource("objects/heli2/main_rotor.obj");
	auto heliSideRotorResource = new ModelResource("objects/heli2/side_rotor.obj");

	/**/
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
	auto playerComponent = new Game::Player();
	player->Add(playerComponent);
	player->Add(camera);

	auto reflectionCamera = new Camera(80.0f, 0.1f, 500.0f, engine->GetScreenWidth(), engine->GetScreenHeight());
	reflectionCamera->SetHudEnabled(false);
	reflectionCamera->EnableRender2Texture();
	reflectionCamera->EnableRenderImage();
	auto attachedCamera = player->CreateChild();
	attachedCamera->GetTransformation()->Rotate(180, vec3(0, 1, 0));
	//a ttachedCamera->GetTransformation()->Translate(vec3(0, -0.1, 0));
	attachedCamera->Add(reflectionCamera);

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
	player->Add(new Game::CameraMovement(spotLight,reflectionCamera));

	engine->SetMainCamera(camera);
	engine->SetUtilityCamera(reflectionCamera);
	camera->GetTransformation()->Translate(vec3(116.0, 60.0, 141.0));
	camera->SetLookAtVector(vec3(0.0, 0.0, 0.0));

	//secondcamera->GetTransformation()->Translate(vec3(30.0, 80.0, 30.0));
	//secondcamera->SetLookAtVector(vec3(0.0, 0.0, 0.0));
	reflectionCamera->SetUpVector(vec3(0.0, 1.0, 0.0));
	reflectionCamera->SetClipping(true, vec4(0, 1, 0, -20));

	PlaceHeli(engine->GetRootEntity(), heliResource, heliMainRotorResource, heliSideRotorResource, vec3(256, 185, 256), false, mapResource, playerComponent);
	PlaceHeli(engine->GetRootEntity(), heliResource, heliMainRotorResource, heliSideRotorResource, vec3(60, 26, 60), true, mapResource, playerComponent);

	PlacePalms(engine->GetRootEntity(), palmResource, mapResource);

	auto map = engine->GetRootEntity()->CreateChild();
	map->Add(new Model(mapResource));

	auto rigidBody = new RigidBody();
	rigidBody->SetStaticness(true);
	rigidBody->SetMaterial(0.5, 0.5, 0.5);
	rigidBody->SetDensity(10);
	map->Add(rigidBody);
	map->Add(new HeightFieldShape(mapResource->GetHeightMap(), mapSize));

	
	auto hudTest = engine->GetRootEntity()->CreateChild();
	hudTest->GetTransformation()->Scale(vec3(0.5, 0.5, 1));
	hudTest->GetTransformation()->Translate(vec3(500, 500, 0));
	auto spriteResource = new SpriteResource(reflectionCamera);
	hudTest->Add(new Model(spriteResource));
	

	engine->Run();

	delete engine;

	return 0;
}
