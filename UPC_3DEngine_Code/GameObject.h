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

	const std::vector<GameObject*>* GetChildren() const;
	const std::vector<Component*>* GetComponents() const;

	//Find Component
	const Component* FindComponentFirst(ComponentType type) const;
	const Component* FindComponentLast(ComponentType type) const;
	void FindComponentVec(std::vector<Component*>& vec, ComponentType type) const;

public:
	std::string name;

private:
	bool Active = true;
	GameObject* parent = nullptr;
	std::vector<GameObject*> children;
	std::vector<Component*> components;
};