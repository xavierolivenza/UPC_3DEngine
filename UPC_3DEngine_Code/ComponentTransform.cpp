#include "imgui-1.51\ImGuizmo.h"
#include "ComponentTransform.h"

ComponentTransform::ComponentTransform(GameObject* parent, bool Active) : Component(parent, Active, 1, ComponentType::Transform_Component)
{
	if (Active) Enable();
	SetPos(float3(0.0f, 0.0f, 0.0f));
	SetScale(float3(1.0f, 1.0f, 1.0f));
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
	return true;
}

void ComponentTransform::DrawComponentImGui()
{
	if (ImGui::CollapsingHeader("Transform Component", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Transform Component Active", &Active);

		ImGui::DragFloat3("Position", &pos[0], 3, ImGuiInputTextFlags_CharsDecimal);
		//ImGui::InputFloat3("Position", &pos[0], 3, ImGuiInputTextFlags_CharsDecimal);

		float3 rot_euler_prev = rot_euler;
		ImGui::DragFloat3("Rotation", &rot_euler[0], 3, ImGuiInputTextFlags_CharsDecimal);
		//ImGui::InputFloat3("Rotation", &rot_euler[0], 3, ImGuiInputTextFlags_CharsDecimal);
		if((rot_euler_prev.x != rot_euler.x) || (rot_euler_prev.y != rot_euler.y) || (rot_euler_prev.z != rot_euler.z))
			rot = rot.FromEulerXYZ(rot_euler.x * DEGTORAD, rot_euler.y * DEGTORAD, rot_euler.z * DEGTORAD);
		
		ImGui::DragFloat3("Scale", &scale[0], 3, ImGuiInputTextFlags_CharsDecimal);
		//ImGui::InputFloat3("Scale", &scale[0], 3, ImGuiInputTextFlags_CharsDecimal);
	}
}

void ComponentTransform::SetPos(float3& new_pos)
{
	pos = new_pos;
}

float3 ComponentTransform::GetPos()
{
	return pos;
}

void ComponentTransform::SetScale(float3& new_scale)
{
	scale = new_scale;
}

float3 ComponentTransform::GetScale()
{
	return scale;
}

void ComponentTransform::SetRot(Quat& new_rot)
{
	rot = new_rot;
	rot_euler = rot.ToEulerXYZ() * RADTODEG;
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
	//TODO Don't do this every time

	float4x4 matrix = float4x4::identity;

	/*
	if (parent->GetParent() != nullptr)
		matrix = matrix * (*parent->GetParent()->GetTransform()->GetMatrix());
	float4x4 local_matrix = float4x4::FromTRS(pos, rot, scale);
	local_matrix.Transpose();
	matrix = matrix * local_matrix;
	*/

	/*
	float4x4 matrix = float4x4::identity;
	if (parent->GetParent() != nullptr)
		matrix = matrix * (*parent->GetParent()->GetTransform()->GetMatrix());
	matrix = matrix * float4x4::FromTRS(pos, rot, scale);
	matrix.Transpose();
	return &matrix;
	*/

	/**/
	matrix = float4x4::FromTRS(pos, rot, scale);
	matrix.Transpose();
	/**/

	return &matrix;
}