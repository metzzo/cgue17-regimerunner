
#include "GameEngine.h"
#include "GameEngine.h"
#include "Transformation.h"
#include "Entity.h"
#include "Camera.h"
#include "Transformation.h"
#include "glm/gtc/matrix_transform.inl"
#include "Rotating.h"
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

using namespace Engine;

void PlacePalms(Entity *child, ModelResource *palmResource, HeightMapResource *map)
{
	auto mapSize = map->GetSize();
	for (auto x = 32; x < mapSize.x; x += 64)
	{
		for (auto z = 32; z < mapSize.z; z+= 64)
		{
			auto palm = child->CreateChild();
			palm->Add(new Model(palmResource));
			palm->Add(new Game::PalmInteraction(x + (rand() % 40 - 20), z + (rand() % 40 - 20), map));
		}
	}
}

void PlaceHeli(Entity *child, ModelResource *heliResource, int num)
{
	std::random_device rd;
	std::mt19937 eng(rd());
	uniform_int_distribution<> distr(1, 600);

	auto x = distr(eng)*1.0f;
	auto z = distr(eng)*1.0f;

	auto heli = child->CreateChild();
	heli->Add(new Model(heliResource));
	heli->GetTransformation()->Translate(vec3(x, 100.0f + num*10, z));
	heli->Add(new Game::HelicopterBehaviour);
}

int main(int argc, char **argv)
{
	auto engine = new GameEngine(1440, 800, string("CGUE"));
	auto mapSize = vec3(512, 128, 512);
	auto mapResource = new HeightMapResource("textures/heightmap.png", mapSize, 1024, 1024);
	mapResource->AddTexture(new TextureResource("textures/sandtext.jpg"), 0, 0.15);
	mapResource->AddTexture(new TextureResource("textures/grasstext.jpg"), 0.15, 0.3);
	mapResource->AddTexture(new TextureResource("textures/stonetext.jpg"), 0.3, 1.0);

	auto palmResource = new ModelResource("objects/palm/palmtree.obj");
	auto heliResource = new ModelResource("objects/heli2/Heli.obj");
	
	auto dirLight = new DirectionalLight(glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, 0.1f, 500.0f), 1024);
	auto dirLightEntity = engine->GetRootEntity()->CreateChild();
	dirLightEntity->Add(dirLight);
	dirLightEntity->GetTransformation()->Translate(vec3(30, 30, 30));
	dirLight->GetCamera()->SetLookAtVector(vec3(0, 0, 0));
	dirLight->SetAmbient(vec3(0.05f, 0.05f, 0.05f));
	dirLight->SetSpecular(vec3(0.2f, 0.2f, 0.2f));
	dirLight->SetDiffuse(vec3(0.1f, 0.1f, 0.1f));

	auto camera = new Camera(80.0f, 0.1f, 500.0f, 1440, 800);
	auto player = engine->GetRootEntity()->CreateChild();
	player->Add(camera);

	auto light = player->CreateChild();
	light->GetTransformation()->Translate(vec3(0, 2.5, 0));

	auto ratio = float(engine->GetScreenWidth()) / float(engine->GetScreenHeight());
	auto spotLight = new SpotLight( 5.0f, 25.0f); //perspective(radians(45.0f), 1.0f, 0.1f, 1000.0f) perspective(radians(180.0f), 1.0f, 0.1f, 25.0f), 1024,
	spotLight->SetAmbient(vec3(0.8f, 0.8f, 0.8f));
	spotLight->SetSpecular(vec3(1, 1, 1));
	spotLight->SetDiffuse(vec3(0.9f, 0.9f, 0.9f));
	light->Add(spotLight); //glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, 0.1f, 500.0f)
	//spotLight->GetCamera()->SetLookAtVector(vec3(30, 0, 30));


	engine->SetMainCamera(camera);
	camera->GetTransformation()->Translate(vec3(30.0, 60.0, 30.0));
	camera->SetLookAtVector(vec3(0.0, 0.0, 0.0));
	player->Add(new Game::CameraMovement(spotLight));

	for (auto i = 0; i < 20; i++) {
		PlaceHeli(engine->GetRootEntity(), heliResource, i);
	}

	PlacePalms(engine->GetRootEntity(), palmResource, mapResource);

	auto map = engine->GetRootEntity()->CreateChild();
	map->Add(new Model(mapResource));

	auto rigidBody = new RigidBody();
	rigidBody->SetStaticness(true);
	rigidBody->SetMaterial(0.5, 0.5, 0.5);
	rigidBody->SetDensity(10);
	map->Add(rigidBody);
	map->Add(new HeightFieldShape(mapResource->GetHeightMap(), mapSize));
	
	engine->Run();

	delete engine;

	return 0;
}
