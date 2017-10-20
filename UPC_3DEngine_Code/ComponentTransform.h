#pragma once
#include "Component.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform(GameObject* parent, bool Active);
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

	const float4x4* GetMatrix() const;

private:
	float3 pos = float3::zero;
	float3 scale = float3::one;
	Quat rot = Quat::identity;
	float3 rot_euler = float3::zero;
	float4x4 matrix = float4x4::identity;
};