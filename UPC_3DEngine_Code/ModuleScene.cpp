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
	//root->CleanUp();
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