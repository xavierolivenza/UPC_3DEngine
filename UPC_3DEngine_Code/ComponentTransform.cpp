#include "imgui-1.51\ImGuizmo.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"

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
	/**/
	ImGuizmo::Enable(!parent->IsStatic());

	if ((parent != nullptr) && (App->engineUI->GetSelectedGameObject() == parent))
	{
		//float4x4 matrix = GetMatrix();
		//ImGuizmo::DrawCube(App->camera->GetViewMatrix().ptr(), App->camera->GetViewProjMatrix().ptr(), matrix.ptr());

		ImGuiIO& io = ImGui::GetIO();
		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
		
		float4x4 viewmatrix = App->camera->GetCameraComp()->frustum.ViewMatrix();
		float4x4 projectionmatrix = App->camera->GetCameraComp()->frustum.ProjectionMatrix();
		viewmatrix.Transpose();
		projectionmatrix.Transpose();

		float4x4 matrix = GetLocalMatrix().Transposed();

		ImGuizmo::DrawCube(viewmatrix.ptr(), projectionmatrix.ptr(), matrix.ptr());

		LOGP("%f, %f, %f, %f", matrix.At(0, 0), matrix.At(0, 1), matrix.At(0, 2), matrix.At(0, 3));
		ImGuizmo::Manipulate(viewmatrix.ptr(), projectionmatrix.ptr(), gizmoOp, ImGuizmo::LOCAL, matrix.ptr());
		LOGP("%f, %f, %f, %f", matrix.At(0, 0), matrix.At(0, 1), matrix.At(0, 2), matrix.At(0, 3));

		if (ImGuizmo::IsUsing())
		{
			matrix.Transpose();
			//matrix = GetMatrix().Inverted() * matrix;
			float3 position = float3::zero;
			float3 scale = float3::zero;
			Quat rotation = Quat::identity;
			matrix.Decompose(position, rotation, scale);
			//LOGP("%f, %f, %f", position.x, position.y, position.z);
			SetPos(position);
			SetRot(rotation);
			SetScale(scale);
		}
	}
	/**/
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

		uint flags = ImGuiInputTextFlags_CharsDecimal;

		if ((parent != nullptr) && parent->IsStatic())
			flags |= ImGuiInputTextFlags_ReadOnly;
		ImGui::InputFloat3("Position", &pos[0], 3, flags);
		if (ImGui::InputFloat3("Rotation", &rot_euler[0], 3, flags))
			rot = rot.FromEulerXYZ(rot_euler.x * DEGTORAD, rot_euler.y * DEGTORAD, rot_euler.z * DEGTORAD);
		ImGui::InputFloat3("Scale", &scale[0], 3, flags);

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
			gizmoOp = ImGuizmo::OPERATION::TRANSLATE;
		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
			gizmoOp = ImGuizmo::OPERATION::ROTATE;
		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
			gizmoOp = ImGuizmo::OPERATION::SCALE;

		/*
		ImGui::DragFloat3("Position", &pos[0], 3, flags);

		if (ImGui::DragFloat3("Rotation", &rot_euler[0], 3, flags))
		rot = rot.FromEulerXYZ(rot_euler.x * DEGTORAD, rot_euler.y * DEGTORAD, rot_euler.z * DEGTORAD);

		ImGui::DragFloat3("Scale", &scale[0], 3, flags);
		*/
	}
}

bool ComponentTransform::SaveComponent(JSON_Object* conf) const
{
	App->parsonjson->SetUInt(conf, "UUID", UUID);
	App->parsonjson->SetUInt(conf, "UUID_Parent", parent->GetUUID());
	App->parsonjson->SetBool(conf, "Active", Active);
	App->parsonjson->SetUInt(conf, "Type", type);
	App->parsonjson->SetFloat3(conf, "Position", pos);
	App->parsonjson->SetFloat3(conf, "Scale", scale);
	App->parsonjson->SetQuat(conf, "Rotation", rot);
	//App->parsonjson->SetFloat4x4(conf, "Matrix", *GetLocalMatrix());
	return true;
}

bool ComponentTransform::LoadComponent(JSON_Object* conf)
{
	UUID = App->parsonjson->GetUInt(conf, "UUID", 0);
	Active = App->parsonjson->GetBool(conf, "Active", true);
	pos = App->parsonjson->GetFloat3(conf, "Position", float3::zero);
	scale = App->parsonjson->GetFloat3(conf, "Scale", float3::zero);
	Quat loaded_rot = App->parsonjson->GetQuat(conf, "Rotation", Quat::identity);
	SetRot(loaded_rot);
	return true;
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

float4x4 ComponentTransform::GetMatrix() const
{
	//TODO Don't do this every time only when modified?

	float4x4 matrix = float4x4::identity;

	std::list<const GameObject*> parents;

	const GameObject* parent_GO = parent;
	while (parent_GO != nullptr)
	{
		parents.push_back(parent_GO);
		parent_GO = parent_GO->GetParent();
	}

	for (std::list<const GameObject*>::reverse_iterator item = parents.rbegin(); item != parents.crend(); ++item)
	{
		float4x4 other = (*item)->GetTransform()->GetLocalMatrix();
		matrix = matrix * other;
	}

	matrix.Transpose();

	return matrix;
}

float4x4 ComponentTransform::GetLocalMatrix() const
{
	float4x4 matrix = float4x4::identity;
	matrix = float4x4::FromTRS(pos, rot, scale);
	//matrix.Transpose();
	return matrix;
}