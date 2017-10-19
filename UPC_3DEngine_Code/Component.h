#pragma once
#include "GameObject.h"
#include "imgui-1.51\imgui.h"

enum ComponentType
{
	Transform_Component,
	Mesh_Component,
	Material_Component,
	Camera_Component,
	Null_ComponentType
};

class Component
{
public:
	Component(GameObject* parent, bool Active, uint replica_limit, ComponentType type);
	~Component();

	virtual bool Enable();
	virtual bool PreUpdate(float dt);
	virtual bool Update(float dt);
	virtual bool PostUpdate(float dt);
	virtual bool Disable();
	virtual bool CleanUp();
	virtual void DrawComponentImGui();
	void SetActive(bool active);
	bool IsActive();
	ComponentType GetType();

protected:
	bool Active = true;
	uint replica_limit = 0; // 0 for no limit
	GameObject* parent = nullptr;
	ComponentType type = ComponentType::Null_ComponentType;
};