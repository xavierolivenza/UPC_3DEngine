#pragma once
#include "Module.h"

class GameObject;
class ComponentCamera;

class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
	void CleanUpScene();

	bool SaveConf(JSON_Object* conf) const;
	bool LoadConf(JSON_Object* conf);

	bool SaveScene(const char* filename) const;
	bool LoadScene(const char* filename);
	bool LoadEditorScene(const char* filename);

	void ImGuiModuleVariables();

	const GameObject* GetRoot() const;
	void AddChildToRoot(GameObject* child);
	bool RemoveChildFromRoot(GameObject* child);

	GameObject* FindGameObjectWithUUID(u32 UUID_ToSearch);

	//Functions to create GameObjects
	GameObject* CreateGameObject(const char* name, bool active = true, bool static_game_object = true);

	const ComponentCamera* GetActiveCamera() const;

	const std::vector<GameObject*>* GetAllSceneGameObjects() const;

private:
	const ComponentCamera* GetActiveCameraIterator(GameObject* node) const;
	void CleanUpGameObjectTree(GameObject* gameobject, bool cleanRoot = true);
	void GetAllSceneGameObjectsCalc();

public:
	bool NewObjectAdded = false;

private:
	GameObject* root = nullptr;
	bool CleanScene = false;
	bool SceneToLoad = false;
	std::string EditorScene_ToLoad;
	std::vector<GameObject*> SceneGameObjects;

};