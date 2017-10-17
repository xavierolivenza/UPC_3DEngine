#include "ComponentTransform.h"

ComponentTransform::ComponentTransform(bool Active, uint replica_limit, ComponentType type) : Component(Active, replica_limit, type)
{
	pos_array = new float[3];
	SetPos(float3(0.0f, 0.0f, 0.0f));
	scale_array = new float[3];
	SetScale(float3(1.0f, 1.0f, 1.0f));
	rot_euler_array = new float[3];
	SetRot(Quat(0.0f, 0.0f, 0.0f, 1.0f));
}

ComponentTransform::~ComponentTransform()
{}

bool ComponentTransform::Enable()
{
	return true;
}

bool ComponentTransform::PreUpdate(float dt)
{
	return true;
}

bool ComponentTransform::Update(float dt)
{
	return true;
}

bool ComponentTransform::PostUpdate(float dt)
{
	return true;
}

bool ComponentTransform::Disable()
{
	return true;
}

bool ComponentTransform::CleanUp()
{
	RELEASE_ARRAY(pos_array);
	RELEASE_ARRAY(scale_array);
	RELEASE_ARRAY(rot_euler_array);
	return true;
}

void ComponentTransform::DrawComponentImGui()
{
	if (ImGui::CollapsingHeader("Transform Component", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::InputFloat3("Position", pos_array);
		ImGui::InputFloat3("Rotation", rot_euler_array);
		ImGui::InputFloat3("Scale", scale_array);
	}
}

void ComponentTransform::SetPos(float3& new_pos)
{
	pos = new_pos;
	pos_array[0] = pos.x;
	pos_array[1] = pos.y;
	pos_array[2] = pos.z;
}

float3 ComponentTransform::GetPos()
{
	return pos;
}

void ComponentTransform::SetScale(float3& new_scale)
{
	scale = new_scale;
	scale_array[0] = scale.x;
	scale_array[1] = scale.y;
	scale_array[2] = scale.z;
}

float3 ComponentTransform::GetScale()
{
	return scale;
}

void ComponentTransform::SetRot(Quat& new_rot)
{
	rot = new_rot;
	rot_euler = rot.ToEulerXYZ();
	rot_euler_array[0] = rot_euler.x;
	rot_euler_array[1] = rot_euler.y;
	rot_euler_array[2] = rot_euler.z;
}

Quat ComponentTransform::GetRot()
{
	return rot;
}

float3 ComponentTransform::GetRotEuler()
{
	return rot_euler;
}