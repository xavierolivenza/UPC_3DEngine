#pragma once
#include "Globals.h"
#include "Application.h"

class Component;
enum ComponentType;

class GameObject
{
public:
	GameObject(const char* name, bool active);
	~GameObject();

	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool CleanUp();

	void SetActive(bool active);

	//Find Component
	const Component* FindComponentFirst(ComponentType type) const;
	const Component* FindComponentLast(ComponentType type) const;
	void FindComponentVec(std::vector<Component*>& vec, ComponentType type) const;

private:
	bool Active = true;
	GameObject* parent = nullptr;
	std::string name;
	std::vector<GameObject*> children;
	std::vector<Component*> components;
};