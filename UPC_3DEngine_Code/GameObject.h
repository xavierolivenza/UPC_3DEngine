#pragma once
#include "Globals.h"
#include "Application.h"

class Component;
enum ComponentType;
class ComponentTransform;
class ComponentMesh;
class ComponentMaterial;
class ComponentCamera;
class ComponentParticleSystem;

class GameObject
{
public:
	GameObject(const char* name, bool active, bool static_game_object);
	~GameObject();

	bool PreUpdate(float dt);
	bool Update(float dt);
	bool ManualUpdate(float dt);
	bool PostUpdate(float dt);
	bool CleanUp();
	bool CleanChildrenVec();

	bool SaveGameObject(JSON_Array* array) const;
	bool LoadGameObject(JSON_Object* conf);

	void SetActive(bool active);
	void SetStatic(bool static_b);
	bool IsActive();
	bool IsStatic();

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
	ComponentParticleSystem* CreateParticleSystemComponent(bool active);
	bool RemoveComponent(Component* component);

	//Find Component
	const Component* FindComponentFirst(ComponentType type) const;
	Component* FindComponentFirstNoConst(ComponentType type) const;
	const Component* FindComponentLast(ComponentType type) const;
	void FindComponentVec(std::vector<Component*>& vec, ComponentType type) const;

	ComponentTransform* GetTransform() const;
	const GameObject* GetParent() const;
	u32 GetUUID() const;
	GameObject* FindGameObjectWithUUID(u32 UUID_ToSearch);

	void GetAllSceneGameObjects(std::vector<GameObject*>& SceneGameObjects) const;

private:
	ComponentTransform* CreateTransformComponent(bool active);
	bool CanCreateOneMoreComponent(ComponentType type);

public:
	float CameraDistance = 0.0f;
	std::string name;
	bool DrawMesh = true; //Used with culling options

private:
	u32 UUID = 0;
	bool Active = true;
	bool Static = true;
	GameObject* parent = nullptr;
	std::vector<GameObject*> children;
	std::vector<Component*> components;
	ComponentTransform* TransformComponent;
};