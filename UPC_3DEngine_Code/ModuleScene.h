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
	void CleanUpGameObjectTree(GameObject* gameobject);

	bool SaveConf(JSON_Object* conf) const;
	bool LoadConf(JSON_Object* conf);

	bool SaveScene(const char* filename) const;
	bool LoadScene(const char* filename);

	void ImGuiModuleVariables();

	const GameObject* GetRoot() const;
	void AddChildToRoot(GameObject* child);
	bool RemoveChildFromRoot(GameObject* child);

	GameObject* FindGameObjectWithUUID(u32 UUID_ToSearch);

	//Functions to create GameObjects
	GameObject* CreateGameObject(const char* name, bool active = true, bool static_game_object = true);

	const ComponentCamera* GetActiveCamera() const;

private:
	const ComponentCamera* GetActiveCameraIterator(GameObject* node) const;

private:
	GameObject* root = nullptr;

};