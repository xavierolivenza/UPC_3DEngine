#pragma once
#include "Module.h"

class GameObject;

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

	bool SaveConf(JSON_Object* conf) const;
	bool LoadConf(JSON_Object* conf);

	void ImGuiModuleVariables();

	const GameObject* GetRoot() const;

	//Functions to create GameObjects

private:
	GameObject* root = nullptr;
};