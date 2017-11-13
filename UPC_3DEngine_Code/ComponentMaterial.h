#pragma once
#include "Component.h"

class ResourceTexture;

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(GameObject* parent, bool Active);
	~ComponentMaterial();

	bool Enable();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool Disable();
	bool CleanUp();
	void DrawComponentImGui();

	void SetResource(uint uuid);

	bool SaveComponent(JSON_Object* conf) const;
	bool LoadComponent(JSON_Object* conf);

public:
	ResourceTexture* resourceTexture = nullptr;
};