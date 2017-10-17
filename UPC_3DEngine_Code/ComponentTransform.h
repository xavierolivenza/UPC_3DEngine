#pragma once
#include "Component.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform(bool Active, uint replica_limit, ComponentType type);
	~ComponentTransform();

	bool Enable();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool Disable();
	bool CleanUp();
	void DrawComponentImGui();

	//Functions to get/set variables
	//We don't make them public and force this functions usage
	//because we can do more stuff than anly copy data, like with
	//rotations, we store quad and euler thanks to the function,
	//and keep both updated
	void SetPos(float3& new_pos);
	float3 GetPos();
	void SetScale(float3& new_scale);
	float3 GetScale();
	void SetRot(Quat& new_rot);
	Quat GetRot();
	float3 GetRotEuler();

private:
	float3 pos = { 0.0f,0.0f,0.0f };
	float* pos_array = nullptr;
	float3 scale = { 1.0f,1.0f,1.0f };
	float* scale_array = nullptr;
	Quat rot = { 0.0f,0.0f,0.0f,1.0f };
	float3 rot_euler = { 0.0f,0.0f,0.0f };
	float* rot_euler_array = nullptr;
};