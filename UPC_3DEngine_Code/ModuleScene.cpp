#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentCamera.h"
#include "ComponentParticleSystem.h"
#include "ComponentTransform.h"

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
	scene_octree.Boundaries(AABB(float3(-1.0f, -1.0f, -1.0f), float3(1.0f, 1.0f, 1.0f)));
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
	{
		for (std::vector<GameObject*>::const_iterator item = SceneGameObjects.cbegin(); item != SceneGameObjects.cend(); ++item)
			(*item)->DrawMesh = false;
	}
	else
	{
		for (std::vector<GameObject*>::const_iterator item = SceneGameObjects.cbegin(); item != SceneGameObjects.cend(); ++item)
			(*item)->DrawMesh = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) octree_draw = !octree_draw;

	root->PreUpdate(dt);
	return UPDATE_CONTINUE;
}

update_status ModuleScene::Update(float dt)
{
	for (std::vector<GameObject*>::const_iterator item = SceneGameObjects.cbegin(); item != SceneGameObjects.cend(); ++item)
	{
		bool Find = false;
		for (std::list<GameObject*>::const_iterator it = SceneParticleSystem.cbegin(); it != SceneParticleSystem.cend(); ++it)
			if (*item == *it)
			{
				Find = true;
				break;
			}
		if (Find == false)
			(*item)->ManualUpdate(dt);
	}
	for (std::list<GameObject*>::const_iterator item = SceneParticleSystem.cbegin(); item != SceneParticleSystem.cend(); ++item)
	{
		float3 Camerapos = App->camera->Position;
		if (App->GetEngineTimeStatus() == EngineTimeStatus::play_unpause)
		{
			const ComponentCamera* camera = App->scene->GetActiveCamera();
			if (camera != nullptr)Camerapos = camera->GetPos();
		}
		float3 Distance = App->camera->Position - ((ComponentTransform*)(*item)->GetTransform())->GetPos();
		(*item)->CameraDistance = Distance.Length();
	}
	SceneParticleSystem.sort([](const GameObject* a, const GameObject* b) { return a->CameraDistance > b->CameraDistance; });
	for (std::list<GameObject*>::const_iterator item = SceneParticleSystem.cbegin(); item != SceneParticleSystem.cend(); ++item)
		(*item)->ManualUpdate(dt);

	if (octree_draw)
		scene_octree.DebugDraw();

	//Firework spawner hardcode, this should be done inside a script
	/**/
	LCG RandGen;
	float Speed = RandGen.Float(25.0f, 50.0f);
	float AngleDiff = 45.0f;
	int Fireworks_num = 4; //from 0 to Fireworks_num
	if (((App->GetEngineTimeStatus() == EngineTimeStatus::play_unpause)) && (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN))
	{
		GameObject* TestParticleSystem = new GameObject("Particle System", true, true);
		ComponentTransform* Transform = (ComponentTransform*)TestParticleSystem->GetTransform();
		float3 EulerRot = Transform->GetRotEuler();
		EulerRot.x = RandGen.Float(-AngleDiff, AngleDiff);
		EulerRot.z = RandGen.Float(-AngleDiff, AngleDiff);
		Quat rot = rot.FromEulerXYZ(EulerRot.x * DEGTORAD, EulerRot.y * DEGTORAD, EulerRot.z * DEGTORAD);
		Transform->SetRot(rot);
		Component* Particle = TestParticleSystem->CreateParticleSystemComponent(true);
		switch (RandGen.Int(0, Fireworks_num)) //Switch different firework particle system with child
		{
		case 0:
			FireworkToAirName_Particle = "..\\Game\\Assets\\Particle System\\Particles\\firework_01";
			FireworkToAirName_Emitter = "..\\Game\\Assets\\Particle System\\Emitter\\firework_01";
			break;
		case 1:
			FireworkToAirName_Particle = "..\\Game\\Assets\\Particle System\\Particles\\firework_02";
			FireworkToAirName_Emitter = "..\\Game\\Assets\\Particle System\\Emitter\\firework_02";
			break;
		case 2:
			FireworkToAirName_Particle = "..\\Game\\Assets\\Particle System\\Particles\\firework_03";
			FireworkToAirName_Emitter = "..\\Game\\Assets\\Particle System\\Emitter\\firework_03";
			break;
		case 3:
			FireworkToAirName_Particle = "..\\Game\\Assets\\Particle System\\Particles\\firework_04";
			FireworkToAirName_Emitter = "..\\Game\\Assets\\Particle System\\Emitter\\firework_04";
			break;
		case 4:
			FireworkToAirName_Particle = "..\\Game\\Assets\\Particle System\\Particles\\firework_05";
			FireworkToAirName_Emitter = "..\\Game\\Assets\\Particle System\\Emitter\\firework_05";
			break;
		}
		((ComponentParticleSystem*)Particle)->LoadParticleResource(FireworkToAirName_Particle.c_str());
		((ComponentParticleSystem*)Particle)->LoadEmitterResource(FireworkToAirName_Emitter.c_str());
		App->scene->AddChildToRoot(TestParticleSystem);
		FireworkSparkleGameObjects.push_back(TestParticleSystem);
	}

	for (std::list<GameObject*>::const_iterator item = FireworkSparkleGameObjects.cbegin(); item != FireworkSparkleGameObjects.cend();)
	{
		ComponentTransform* Transform = (ComponentTransform*)(*item)->GetTransform();
		float3 Position = Transform->GetPos();
		Quat Rotation = Transform->GetRot();
		float3 Direction = float3(0.0f, 1.0f, 0.0f);
		Direction = Rotation.Transform(Direction);
		Position += (Direction * dt * Speed);
		Transform->SetPos(Position);
		if (((ComponentParticleSystem*)(*item)->FindComponentFirst(ComponentType::ParticleSystem_Component))->IsEmitterDead())
			item = FireworkSparkleGameObjects.erase(item);
		else ++item;
	}
	/**/
	if (((App->GetEngineTimeStatus() == EngineTimeStatus::play_unpause)) && (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN))
	{
		GameObject* TestParticleSystem = new GameObject("Particle System", true, true);
		ComponentTransform* Transform = (ComponentTransform*)TestParticleSystem->GetTransform();
		float3 EulerRot = Transform->GetRotEuler();
		EulerRot.x = RandGen.Float(-AngleDiff, AngleDiff);
		EulerRot.z = RandGen.Float(-AngleDiff, AngleDiff);
		Quat rot = rot.FromEulerXYZ(EulerRot.x * DEGTORAD, EulerRot.y * DEGTORAD, EulerRot.z * DEGTORAD);
		Transform->SetRot(rot);
		Component* Particle = TestParticleSystem->CreateParticleSystemComponent(true);
		switch (RandGen.Int(0, Fireworks_num)) //Switch different firework (to the air) particle system
		{
		case 0:
			FireworkToAirName_Particle = "..\\Game\\Assets\\Particle System\\Particles\\firework_01_toair";
			FireworkToAirName_Emitter = "..\\Game\\Assets\\Particle System\\Emitter\\firework_01_toair";
			break;
		case 1:
			FireworkToAirName_Particle = "..\\Game\\Assets\\Particle System\\Particles\\firework_02_toair";
			FireworkToAirName_Emitter = "..\\Game\\Assets\\Particle System\\Emitter\\firework_02_toair";
			break;
		case 2:
			FireworkToAirName_Particle = "..\\Game\\Assets\\Particle System\\Particles\\firework_03_toair";
			FireworkToAirName_Emitter = "..\\Game\\Assets\\Particle System\\Emitter\\firework_03_toair";
			break;
		case 3:
			FireworkToAirName_Particle = "..\\Game\\Assets\\Particle System\\Particles\\firework_04_toair";
			FireworkToAirName_Emitter = "..\\Game\\Assets\\Particle System\\Emitter\\firework_04_toair";
			break;
		case 4:
			FireworkToAirName_Particle = "..\\Game\\Assets\\Particle System\\Particles\\firework_05_toair";
			FireworkToAirName_Emitter = "..\\Game\\Assets\\Particle System\\Emitter\\firework_05_toair";
			break;
		}
		((ComponentParticleSystem*)Particle)->LoadParticleResource(FireworkToAirName_Particle.c_str());
		((ComponentParticleSystem*)Particle)->LoadEmitterResource(FireworkToAirName_Emitter.c_str());
		App->scene->AddChildToRoot(TestParticleSystem);
		FireworkSparkleGameObjects_NoChild.push_back(TestParticleSystem);
	}

	for (std::list<GameObject*>::const_iterator item = FireworkSparkleGameObjects_NoChild.cbegin(); item != FireworkSparkleGameObjects_NoChild.cend();)
	{
		ComponentTransform* Transform = (ComponentTransform*)(*item)->GetTransform();
		float3 Position = Transform->GetPos();
		Quat Rotation = Transform->GetRot();
		float3 Direction = float3(0.0f, 1.0f, 0.0f);
		Direction = Rotation.Transform(Direction);
		Position += (Direction * dt * Speed);
		Transform->SetPos(Position);
		if (((ComponentParticleSystem*)(*item)->FindComponentFirst(ComponentType::ParticleSystem_Component))->IsEmitterDead())
		{
			GameObject* TestParticleSystem = new GameObject("Particle System", true, true);
			ComponentTransform* Transform = (ComponentTransform*)TestParticleSystem->GetTransform();
			Transform->SetPos(((ComponentTransform*)(*item)->GetTransform())->GetPos());
			Component* Particle = TestParticleSystem->CreateParticleSystemComponent(true);
			switch (RandGen.Int(0, Fireworks_num)) //Switch different firework (explosion) particle system
			{
			case 0:
				FireworkToAirName_Particle = "..\\Game\\Assets\\Particle System\\Particles\\firework_01_explosion";
				FireworkToAirName_Emitter = "..\\Game\\Assets\\Particle System\\Emitter\\firework_01_explosion";
				break;
			case 1:
				FireworkToAirName_Particle = "..\\Game\\Assets\\Particle System\\Particles\\firework_02_explosion";
				FireworkToAirName_Emitter = "..\\Game\\Assets\\Particle System\\Emitter\\firework_02_explosion";
				break;
			case 2:
				FireworkToAirName_Particle = "..\\Game\\Assets\\Particle System\\Particles\\firework_03_explosion";
				FireworkToAirName_Emitter = "..\\Game\\Assets\\Particle System\\Emitter\\firework_03_explosion";
				break;
			case 3:
				FireworkToAirName_Particle = "..\\Game\\Assets\\Particle System\\Particles\\firework_04_explosion";
				FireworkToAirName_Emitter = "..\\Game\\Assets\\Particle System\\Emitter\\firework_04_explosion";
				break;
			case 4:
				FireworkToAirName_Particle = "..\\Game\\Assets\\Particle System\\Particles\\firework_05_explosion";
				FireworkToAirName_Emitter = "..\\Game\\Assets\\Particle System\\Emitter\\firework_05_explosion";
				break;
			}
			((ComponentParticleSystem*)Particle)->LoadParticleResource(FireworkToAirName_Particle.c_str());
			((ComponentParticleSystem*)Particle)->LoadEmitterResource(FireworkToAirName_Emitter.c_str());
			App->scene->AddChildToRoot(TestParticleSystem);
			item = FireworkSparkleGameObjects_NoChild.erase(item);
		}
		else ++item;
	}
	/**/

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleScene::PostUpdate(float dt)
{
	root->PostUpdate(dt);
	if (CleanScene)
	{
		CleanUpGameObjectTree(root, false);
		scene_octree.Clear(false);
		FireworkSparkleGameObjects.clear();
		FireworkSparkleGameObjects_NoChild.clear();
		CleanScene = false;
	}
	if (SceneToLoad)
	{
		//First reset resources
		App->resources->CleanUp();
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

	//No necessary to RELEASE memory because this vector and lists store pointers to game objects, stored and released in CleanUpGameObjectTree
	SceneGameObjects.clear();
	SceneParticleSystem.clear();
	FireworkSparkleGameObjects.clear();
	FireworkSparkleGameObjects_NoChild.clear();
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

	ParsonJSON* parsonjson = new ParsonJSON(filename, false, true, false);
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
	ParsonJSON* parsonjson = new ParsonJSON(filename, false, true, true);
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
	SceneParticleSystem.clear();
	for (std::vector<GameObject*>::const_iterator item = SceneGameObjects.cbegin(); item != SceneGameObjects.cend(); ++item)
	{
		const Component* comp = (*item)->FindComponentFirst(ComponentType::ParticleSystem_Component);
		if (comp != nullptr)
			SceneParticleSystem.push_back(*item);
	}
}