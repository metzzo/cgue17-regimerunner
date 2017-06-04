
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
#include "SpriteResource.h"

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

void PlaceHeli(Entity *child, ModelResource *heliResource, int num)
{
	auto engine = child->GetEngine();
	auto ratio = float(engine->GetScreenWidth()) / float(engine->GetScreenHeight());

	std::random_device rd;
	std::mt19937 eng(rd());
	uniform_int_distribution<> distr(1, 600);

	auto x = distr(eng)*1.0f;
	auto z = distr(eng)*1.0f;

	auto heli = child->CreateChild();
	//heli->Add(new Model(heliResource));
	heli->GetTransformation()->Translate(vec3(256, 130.0f + num*10, 256));
	heli->Add(new Game::HelicopterBehaviour);

	auto spotLight = new SpotLight(perspective(radians(50.0f), 1.0f, 0.1f, 500.0f), 2048, 45.0f, 50.0f); //new SpotLight(glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, 0.1f, 500.0f), 1024, 5.0f, 10.0f); // new SpotLight(5.0, 30.0); //
	spotLight->SetAmbient(vec3(0, 0, 0));
	spotLight->SetSpecular(vec3(1, 1, 1));
	spotLight->SetDiffuse(vec3(0.9f, 0.9f, 0.9f));
	spotLight->SetLinear(0.0014f);
	spotLight->SetQuadratic(0.00007f);
	heli->Add(spotLight);
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
	auto heliResource = new ModelResource("objects/heli2/Heli.obj");
	
	auto dirLight = new DirectionalLight();
	auto dirLightEntity = engine->GetRootEntity()->CreateChild();
	dirLightEntity->Add(dirLight);
	dirLightEntity->GetTransformation()->Translate(vec3(256, 256, 512));
	dirLight->SetAmbient(vec3(0, 0, 0));
	dirLight->SetSpecular(vec3(0.2f, 0.2f, 0.2f));
	dirLight->SetDiffuse(vec3(0.2f, 0.2f, 0.2f));
	dirLight->SetLookAtVector(vec3(0, 0, 0));

	auto ratio = float(engine->GetScreenWidth()) / float(engine->GetScreenHeight());
	auto projectionMatrix = perspective(radians(80.0f), ratio, 0.1f, 500.0f);

	auto camera = new Camera(projectionMatrix, 1440, 800);
	camera->SetHudProjectionMatrix(glm::ortho(0.0f, GLfloat(engine->GetScreenWidth()), GLfloat(engine->GetScreenHeight()), 0.0f, -1.0f, 1.0f));
	auto player = engine->GetRootEntity()->CreateChild();
	player->Add(camera);


	auto hudTest = engine->GetRootEntity()->CreateChild();
	auto spriteResource = new SpriteResource(new TextureResource("textures/sandtext.jpg"));
	hudTest->Add(new Model(spriteResource));

	auto light = player->CreateChild();
	light->GetTransformation()->Translate(vec3(0, 2.5, 0));

	auto spotLight = new SpotLight( 5.0f, 35.0f); //perspective(radians(45.0f), 1.0f, 0.1f, 1000.0f) perspective(radians(180.0f), 1.0f, 0.1f, 25.0f), 1024,
	spotLight->SetAmbient(vec3(0.8f, 0.8f, 0.8f));
	spotLight->SetSpecular(vec3(1, 1, 1));
	spotLight->SetDiffuse(vec3(0.9f, 0.9f, 0.9f));
	spotLight->SetLinear(0.007f);
	spotLight->SetQuadratic(0.002f);
	light->Add(spotLight); //glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, 0.1f, 500.0f)
	//spotLight->GetCamera()->SetLookAtVector(vec3(30, 0, 30));


	engine->SetMainCamera(camera);
	camera->GetTransformation()->Translate(vec3(30.0, 60.0, 30.0));
	camera->SetLookAtVector(vec3(0.0, 0.0, 0.0));
	player->Add(new Game::CameraMovement(spotLight));

	for (auto i = 0; i < 1; i++) {
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
