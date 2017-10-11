#pragma once
#include "Globals.h"
#include "Application.h"
//#include "Component.h"

class Component;

class GameObject
{
public:
	GameObject();
	~GameObject();

	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool CleanUp();

	void SetActive(bool active);

	//Find Component

private:
	bool Active = true;
	GameObject* parent = nullptr;
	std::string name;
	std::vector<GameObject*> children;
	std::vector<Component*> components;
};