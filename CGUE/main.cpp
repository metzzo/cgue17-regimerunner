

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

using namespace Engine;

int main(int argc, char **argv)
{
	auto engine = new GameEngine(1024, 640, string("CGUE"));
	auto modelResource = new ModelResource("objects/mapobj.obj");
	auto heightMapResource = new TextureResource("textures/heightmap.png");

	auto camera = new Camera(45.0f, 0.1f, 100.0f, 1024, 640);
	auto player = engine->GetRootEntity()->CreateChild();
	player->Add(camera);

	engine->SetMainCamera(camera);
	camera->GetTransformation()->Translate(vec3(30.0, 30.0, 30.0));
	camera->SetLookAtVector(vec3(0.0, 0.0, 0.0));
	player->Add(new Game::CameraMovement);


	auto map = engine->GetRootEntity()->CreateChild();
	map->Add(new Model(modelResource));

	auto rigidBody = new RigidBody();
	rigidBody->SetStaticness(true);
	rigidBody->SetMaterial(0.5, 0.5, 0.5);
	rigidBody->SetDensity(10);
	map->Add(rigidBody);
	map->Add(new HeightFieldShape(heightMapResource));
	//map->GetTransformation()->Scale(vec3(0.1f, 0.1f, 0.1f));
	//cube->Add(new Game::Rotating());
	
	auto light = engine->GetRootEntity()->CreateChild();
	auto spotLight = new SpotLight();
	light->Add(spotLight);
	spotLight->GetCamera()->GetTransformation()->Translate(vec3(4.0, 3.0, 3.0));
	spotLight->GetCamera()->SetLookAtVector(vec3(0.0, 0.0, 0.0));

	engine->Run();

	delete modelResource;
	delete engine;

	return 0;
}
