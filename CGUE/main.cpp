

#include "GameEngine.h"
#include "GameEngine.h"
#include "Transformation.h"
#include "Entity.h"
#include "Camera.h"
#include "Transformation.h"
#include "glm/gtc/matrix_transform.inl"
#include "Rotating.h"
#include "Texture.h"
#include "SpotLight.h"
#include "Model.h"

using namespace Engine;

int main(int argc, char **argv)
{
	auto engine = new GameEngine(640, 480, string("CGUE"));

	auto camera = new Camera(45.0f, 0.1f, 100.0f, 640, 480);
	engine->GetRootEntity()->CreateChild()->Add(camera);
	engine->SetMainCamera(camera);
	camera->GetTransformation()->Translate(vec3(4.0, 3.0, 3.0));
	camera->SetLookAtVector(vec3(0.0, 0.0, 0.0));

	auto cube = engine->GetRootEntity()->CreateChild();
	cube->Add(new Model("objects/nanosuit/nanosuit.obj"));
	cube->GetTransformation()->Scale(vec3(0.1f, 0.1f, 0.1f));
	//cube->Add(new Game::Rotating());

	cube = engine->GetRootEntity()->CreateChild();
	cube->Add(new Model("objects/nanosuit/nanosuit.obj"));
	cube->Add(new Game::Rotating());
	cube->GetTransformation()->Translate(vec3(2.0f, .3f, 0.4f));
	cube->GetTransformation()->Scale(vec3(0.1f, 0.1f, 0.1f));
	
	auto light = engine->GetRootEntity()->CreateChild();
	auto spotLight = new SpotLight();
	light->Add(spotLight);
	spotLight->GetCamera()->GetTransformation()->Translate(vec3(4.0, 3.0, 3.0));
	spotLight->GetCamera()->SetLookAtVector(vec3(0.0, 0.0, 0.0));

	engine->Run();

	delete engine;

	return 0;
}
