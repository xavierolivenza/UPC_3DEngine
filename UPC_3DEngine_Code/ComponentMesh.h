#pragma once
#include "Component.h"

class ResourceMesh;

class ComponentMesh : public Component
{
	friend class OctreeNode;

public:
	ComponentMesh(GameObject* parent, bool Active);
	~ComponentMesh();

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

	void GetTransformedAABB(AABB& TransformedBox) const;

public:
	ResourceMesh* resourceMesh = nullptr;

private:
	bool DebugDrawAABB = false;
	bool DebugDrawOBB = false;
};