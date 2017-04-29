

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

using namespace Engine;

int main(int argc, char **argv)
{
	auto engine = new GameEngine(1024, 640, string("CGUE"));
	auto modelResource = new ModelResource("objects/mapobj.obj");

	auto camera = new Camera(45.0f, 0.1f, 100.0f, 1024, 640);
	engine->GetRootEntity()->CreateChild()->Add(camera);
	engine->SetMainCamera(camera);
	camera->GetTransformation()->Translate(vec3(6.0, 2.0, 1.0));
	camera->SetLookAtVector(vec3(0.0, 1.0, 0.0));
	auto camentity = camera->GetEntity();
	camentity->Add(new Game::CameraMovement);

	auto cube = engine->GetRootEntity()->CreateChild();
	cube->Add(new Model(modelResource));
	cube->GetTransformation()->Scale(vec3(0.1f, 0.1f, 0.1f));
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
