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
	Component();
	~Component();

	virtual bool Enable();
	virtual bool PreUpdate(float dt);
	virtual bool Update(float dt);
	virtual bool PostUpdate(float dt);
	virtual bool Disable();
	virtual bool CleanUp();
	virtual void DrawComponentImGui();
	void SetActive(bool active);
	ComponentType GetType();

protected:
	bool Active = true;
	ComponentType type = ComponentType::Null_ComponentType;
};