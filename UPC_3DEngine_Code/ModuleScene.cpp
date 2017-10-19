#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "scene";
}

// Destructor
ModuleScene::~ModuleScene()
{}

// Called before render is available
bool ModuleScene::Init()
{
	LOGP("Creating Module Scene");
	bool ret = true;
	root = new GameObject("Root", true, true);
	TestCamera = new GameObject("Camera", true, true);
	TestCamera->CreateTransformComponent(TestCamera, true);
	TestCamera->CreateCameraComponent(TestCamera, true);
	AddChildToRoot(TestCamera);
	return ret;
}

bool ModuleScene::Start()
{
	return true;
}

// PreUpdate: clear buffer
update_status ModuleScene::PreUpdate(float dt)
{
	root->PreUpdate(dt);
	return UPDATE_CONTINUE;
}

update_status ModuleScene::Update(float dt)
{
	root->Update(dt);
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleScene::PostUpdate(float dt)
{
	root->PostUpdate(dt);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleScene::CleanUp()
{
	LOGP("Destroying Module Scene");
	root->CleanUp();
	RELEASE(root);
	TestCamera->CleanUp();
	RELEASE(TestCamera);
	return true;
}

bool ModuleScene::SaveConf(JSON_Object* conf) const
{
	return true;
}

bool ModuleScene::LoadConf(JSON_Object* conf)
{
	return true;
}

void ModuleScene::ImGuiModuleVariables()
{
	
}

const GameObject* ModuleScene::GetRoot() const
{
	return root;
}

void ModuleScene::AddChildToRoot(GameObject* child)
{
	root->AddChild(child);
}

bool ModuleScene::RemoveChildFromRoot(GameObject* child)
{
	return root->RemoveChild(child);
}

GameObject* ModuleScene::CreateGameObject(const char* name, bool active, bool static_game_object)
{
	return new GameObject(name, active, static_game_object);
}