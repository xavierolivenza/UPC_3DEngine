#include "ComponentTransform.h"

ComponentTransform::ComponentTransform(GameObject* parent, bool Active) : Component(parent, Active, 1, ComponentType::Transform_Component)
{
	if (Active) Enable();
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
	return true;
}

void ComponentTransform::DrawComponentImGui()
{
	if (ImGui::CollapsingHeader("Transform Component", ImGuiTreeNodeFlags_DefaultOpen))
	{
		float3 pos_prev = pos;
		ImGui::DragFloat3("Position", &pos[0], 3, ImGuiInputTextFlags_CharsDecimal);
		//ImGui::InputFloat3("Position", &pos[0], 3, ImGuiInputTextFlags_CharsDecimal);
		if ((pos_prev.x != pos.x) || (pos_prev.y != pos.y) || (pos_prev.z != pos.z))
			change = true;

		float3 euler_prev = rot_euler;
		ImGui::DragFloat3("Rotation", &rot_euler[0], 3, ImGuiInputTextFlags_CharsDecimal);
		//ImGui::InputFloat3("Rotation", &rot_euler[0], 3, ImGuiInputTextFlags_CharsDecimal);
		if ((euler_prev.x != rot_euler.x) || (euler_prev.y != rot_euler.y) || (euler_prev.z != rot_euler.z))
		{
			rot.FromEulerXYZ(rot_euler.x, rot_euler.y, rot_euler.z);
			change = true;
		}
		
		float3 scale_prev = scale;
		ImGui::DragFloat3("Scale", &scale[0], 3, ImGuiInputTextFlags_CharsDecimal);
		//ImGui::InputFloat3("Scale", &scale[0], 3, ImGuiInputTextFlags_CharsDecimal);
		if ((scale_prev.x != scale.x) || (scale_prev.y != scale.y) || (scale_prev.z != scale.z))
			change = true;

		UpdateMatrix();
	}
}

void ComponentTransform::SetPos(float3& new_pos)
{
	pos = new_pos;
	change = true;
	UpdateMatrix();
}

float3 ComponentTransform::GetPos()
{
	return pos;
}

void ComponentTransform::SetScale(float3& new_scale)
{
	scale = new_scale;
	change = true;
	UpdateMatrix();
}

float3 ComponentTransform::GetScale()
{
	return scale;
}

void ComponentTransform::SetRot(Quat& new_rot)
{
	rot = new_rot;
	rot_euler = rot.ToEulerXYZ() * RADTODEG;
	change = true;
	UpdateMatrix();
}

Quat ComponentTransform::GetRot()
{
	return rot;
}

float3 ComponentTransform::GetRotEuler()
{
	return rot_euler;
}

const float4x4* ComponentTransform::GetMatrix() const
{
	return &matrix;
}

void ComponentTransform::UpdateMatrix()
{
	if (change)
	{
		matrix = float4x4::FromTRS(pos, float4x4::FromQuat(rot), scale);
		matrix.Transpose();
		change = false;
	}
}