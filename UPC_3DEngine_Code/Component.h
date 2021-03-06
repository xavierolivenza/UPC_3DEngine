#pragma once
#include "GameObject.h"
#include "imgui-1.51\imgui.h"

enum ComponentType
{
	Transform_Component,
	Mesh_Component,
	Material_Component,
	Camera_Component,
	ParticleSystem_Component,
	Null_ComponentType
};

class Component
{
	friend ComponentCamera;
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
	virtual bool SaveComponent(JSON_Object* conf) const;
	virtual bool LoadComponent(JSON_Object* conf);
	void SetActive(bool active);
	bool IsActive() const;
	ComponentType GetType() const;
	uint GetReplicaLimit() const;

protected:
	bool Active = true;
	uint replica_limit = 0; // 0 for no limit
	GameObject* parent = nullptr;
	ComponentType type = ComponentType::Null_ComponentType;
	uint UUID = 0;
};