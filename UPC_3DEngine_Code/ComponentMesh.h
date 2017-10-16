#pragma once
#include "Component.h"

class ComponentMesh : public Component
{
public:
	ComponentMesh();
	~ComponentMesh();

	bool Enable();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool Disable();
	bool CleanUp();
	void DrawComponentImGui();
};