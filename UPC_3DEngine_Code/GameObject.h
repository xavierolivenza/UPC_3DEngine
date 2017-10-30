#pragma once
#include "Globals.h"
#include "Application.h"

class Component;
enum ComponentType;
class ComponentTransform;
class ComponentMesh;
class ComponentMaterial;
class ComponentCamera;

class GameObject
{
public:
	GameObject(const char* name, bool active, bool static_game_object);
	~GameObject();

	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool CleanUp();

	bool SaveGameObject(JSON_Object* conf) const;
	bool LoadGameObject(JSON_Object* conf);

	void SetActive(bool active);
	bool IsActive();

	const std::vector<GameObject*>* GetChildren() const;
	const std::vector<Component*>* GetComponents() const;

	void DrawGameObjectImGui();
	void DrawComponentImGui();

	void AddChild(GameObject* child);
	bool RemoveChild(GameObject* child);

	//Create/Remove Component
	//ComponentTransform* CreateTransformComponent(bool active);
	ComponentMesh* CreateMeshComponent(bool active);
	ComponentMaterial* CreateMaterialComponent(bool active);
	ComponentCamera* CreateCameraComponent(bool active);
	bool RemoveComponent(Component* component);

	//Find Component
	const Component* FindComponentFirst(ComponentType type) const;
	const Component* FindComponentLast(ComponentType type) const;
	void FindComponentVec(std::vector<Component*>& vec, ComponentType type) const;

	ComponentTransform* GetTransform() const;
	const GameObject* GetParent() const;

private:
	ComponentTransform* CreateTransformComponent(bool active);

public:
	std::string name;

private:
	u32 UUID = 0;
	bool Active = true;
	bool Static = true;
	GameObject* parent = nullptr;
	std::vector<GameObject*> children;
	std::vector<Component*> components;
	ComponentTransform* TransformComponent;
};