#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentCamera.h"

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
	GameObject* TestCamera = new GameObject("Camera", true, true);
	TestCamera->CreateCameraComponent(true);
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
	if (NewObjectAdded)
		GetAllSceneGameObjectsCalc();
	//SetAll GameObjects DrawMesh to false(camera culling option will turn them on if in fov)
	const ComponentCamera* camera = App->scene->GetActiveCamera();
	if ((camera != nullptr) && (camera->IsCulling()))
		for (std::vector<GameObject*>::const_iterator item = SceneGameObjects.cbegin(); item != SceneGameObjects.cend(); ++item)
			(*item)->DrawMesh = false;

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
	if (CleanScene)
	{
		CleanUpGameObjectTree(root, false);
		CleanScene = false;
	}
	if (SceneToLoad)
	{
		LoadScene(EditorScene_ToLoad.c_str());
		EditorScene_ToLoad.clear();
		SceneToLoad = false;
	}
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleScene::CleanUp()
{
	LOGP("Destroying Module Scene");
	CleanUpGameObjectTree(root); //Root cleared inside, no delete/RELEASE needed here
	return true;
}

void ModuleScene::CleanUpScene()
{
	CleanScene = true;
}

bool ModuleScene::LoadEditorScene(const char* filename)
{
	EditorScene_ToLoad = filename;
	SceneToLoad = true;
	return true;
}

void ModuleScene::CleanUpGameObjectTree(GameObject* gameobject, bool cleanRoot)
{
	if (gameobject != nullptr)
	{
		//Iterate Children
		const std::vector<GameObject*>* vectorChildren = gameobject->GetChildren();
		for (std::vector<GameObject*>::const_iterator item = vectorChildren->begin(); item != vectorChildren->cend(); ++item)
		{
			(*item)->CleanUp();
			CleanUpGameObjectTree(*item);
		}
		gameobject->CleanChildrenVec();
		//Once gameobject is cleaned, release it.
		if (cleanRoot)
		{
			gameobject->CleanUp();
			RELEASE(gameobject);
		}
		NewObjectAdded = true;
	}
}

bool ModuleScene::SaveConf(JSON_Object* conf) const
{
	return true;
}

bool ModuleScene::LoadConf(JSON_Object* conf)
{
	return true;
}

bool ModuleScene::SaveScene(const char* filename) const
{
	bool ret = true;

	ParsonJSON* parsonjson = new ParsonJSON(filename, true, false);
	ret = parsonjson->Init();
	if (ret)
		ret = parsonjson->SaveScene(root);
	RELEASE(parsonjson);

	if (ret) LOGP("SaveScene Success, file: %s", filename);
	else LOGP("SaveScene Failure, file: %s", filename);
	return ret;
}

bool ModuleScene::LoadScene(const char* filename)
{
	bool ret = true;
	ParsonJSON* parsonjson = new ParsonJSON(filename, true, true);
	ret = parsonjson->Init();
	if (ret)
		ret = parsonjson->LoadScene(root);
	RELEASE(parsonjson);
	if (ret) LOGP("LoadScene Success, file: %s", filename);
	else LOGP("LoadScene Failure, file: %s", filename);
	return ret;
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
	NewObjectAdded = true;
}

bool ModuleScene::RemoveChildFromRoot(GameObject* child)
{
	return root->RemoveChild(child);
	App->scene->NewObjectAdded = true;
}

GameObject* ModuleScene::FindGameObjectWithUUID(u32 UUID_ToSearch)
{
	return root->FindGameObjectWithUUID(UUID_ToSearch);
}

GameObject* ModuleScene::CreateGameObject(const char* name, bool active, bool static_game_object)
{
	return new GameObject(name, active, static_game_object);
}

const ComponentCamera* ModuleScene::GetActiveCamera() const
{
	const ComponentCamera* camera = nullptr;
	if (root != nullptr)
		camera = GetActiveCameraIterator(root);
	return camera;
}

const ComponentCamera* ModuleScene::GetActiveCameraIterator(GameObject* node) const
{
	const ComponentCamera* camera = nullptr;
	const std::vector<GameObject*>* children = node->GetChildren();
	for (std::vector<GameObject*>::const_iterator item = children->cbegin(); item != children->cend(); ++item)
	{
		const Component* camera_comp_found = (*item)->FindComponentFirst(ComponentType::Camera_Component);
		if ((camera_comp_found != nullptr) && (((ComponentCamera*)camera_comp_found)->IsMainCamera()))
			return (ComponentCamera*)camera_comp_found;
		else
		{
			camera = GetActiveCameraIterator(*item);
			if ((camera != nullptr) && (camera->IsMainCamera()))
				return camera;
		}
	}
	return nullptr;
}

const std::vector<GameObject*>* ModuleScene::GetAllSceneGameObjects() const
{
	return &SceneGameObjects;
}

void ModuleScene::GetAllSceneGameObjectsCalc()
{
	SceneGameObjects.clear();
	root->GetAllSceneGameObjects(SceneGameObjects);
}